/*
 * A061180  Fifth column (m=4) of triangle A060920 (bisection of Fibonacci
 *          triangle, even part).
 * https://oeis.org/A061180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A060920(n+4,4) Recurrence (order 10, minimal for the
 *            rational generating function): a(n) = 15*a(n-1) – 95*a(n-2) +
 *            330*a(n-3) – 685*a(n-4) + 873*a(n-5) – 685*a(n-6) + 330*a(n-7) –
 *            95*a(n-8) + 15*a(n-9) – a(n-10) with a(0) … a(9) as given below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values a(0) … a(9) */
    static const seq_t init[10] = {
        (seq_t)1ULL,        /* a(0) */
        (seq_t)20ULL,       /* a(1) */
        (seq_t)190ULL,      /* a(2) */
        (seq_t)1295ULL,     /* a(3) */
        (seq_t)7285ULL,     /* a(4) */
        (seq_t)36122ULL,    /* a(5) */
        (seq_t)163730ULL,   /* a(6) */
        (seq_t)693835ULL,   /* a(7) */
        (seq_t)2790100ULL,  /* a(8) */
        (seq_t)10758050ULL  /* a(9) */
    };

    /* coefficients of the recurrence: the first is for a(n-1), the last for a(n-10) */
    static const int coeff[10] = {15, -95, 330, -685, 873, -685, 330, -95, 15, -1};

    if (n < 10)
        return init[(size_t)n];

    /* buffer holds previous 10 terms:
       buf[0] = a(k-1), buf[1] = a(k-2), … , buf[9] = a(k-10)   */
    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i)
        buf[i] = init[9 - i];

    for (idx_t k = 10; k <= n; ++k) {
        seq_t pos = 0;   /* sum of positive contributions */
        seq_t neg = 0;   /* sum of negative contributions */

        for (int i = 0; i < 10; ++i) {
            const int ci = coeff[i];
            seq_t term = buf[i] * (seq_t)(ci >= 0 ? ci : -ci);
            if (ci >= 0)
                pos += term;
            else
                neg += term;
        }

        seq_t val = pos - neg; /* guaranteed pos >= neg for integer arithmetic */

        /* shift buffer and insert new value */
        for (int i = 9; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = val;
    }

    return buf[0];
}
