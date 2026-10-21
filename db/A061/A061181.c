/*
 * A061181  Sixth column (m=5) of triangle A060920 (bisection of Fibonacci
 *          triangle, even part).
 * https://oeis.org/A061181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) -141*a(n-2) +630*a(n-3) -1770*a(n-4)
 *            +3258*a(n-5) -3989*a(n-6) +3258*a(n-7) -1770*a(n-8) +630*a(n-9)
 *            -141*a(n-10) +18*a(n-11) -a(n-12); Initial values: a(0)=1,
 *            a(1)=27, a(2)=315, a(3)=2534, a(4)=16407, a(5)=91959,
 *            a(6)=464723, a(7)=2171850, a(8)=9546570, a(9)=39940460,
 *            a(10)=1604...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 12 initial terms */
    static const seq_t init[12] = {
        1ULL, 27ULL, 315ULL, 2534ULL, 16407ULL,
        91959ULL, 464723ULL, 2171850ULL, 9546570ULL,
        39940460ULL, 160437690ULL, 622844730ULL
    };

    if (n < 12)
        return init[n];

    /* coefficients of the recurrence */
    static const int coeff[12] = {
        18, -141, 630, -1770, 3258,
        -3989, 3258, -1770, 630, -141,
        18, -1
    };

    /* previous 12 terms: prev[0] = a(k-1), prev[1] = a(k-2), …, prev[11] = a(k-12) */
    seq_t prev[12];
    for (int i = 0; i < 12; ++i)
        prev[11 - i] = init[i];

#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ >= 16)
    /* use signed 128‑bit intermediate if available */
    for (idx_t k = 12; k <= n; ++k) {
        __int128 sum = 0;
        for (int i = 0; i < 12; ++i)
            sum += (__int128)coeff[i] * (__int128)prev[i];
        seq_t val = (seq_t)sum;
        for (int i = 11; i > 0; --i)
            prev[i] = prev[i - 1];
        prev[0] = val;
    }
#else
    /* fallback: use signed long long intermediate (sufficient for 64‑bit seq_t) */
    for (idx_t k = 12; k <= n; ++k) {
        long long sum = 0;
        for (int i = 0; i < 12; ++i)
            sum += (long long)coeff[i] * (long long)prev[i];
        seq_t val = (seq_t)sum;
        for (int i = 11; i > 0; --i)
            prev[i] = prev[i - 1];
        prev[0] = val;
    }
#endif

    return prev[0];
}
