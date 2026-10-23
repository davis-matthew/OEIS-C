/*
 * A138619  Number of permutations of length n which are contained in a pin
 *          sequence.
 * https://oeis.org/A138619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 8*a(n-1) - 19*a(n-2)
 *            + 26*a(n-3) - 14*a(n-4) + 12*a(n-5) + 8*a(n-6) - 20*a(n-7) +
 *            8*a(n-8) with initial values a(1)=1, a(2)=2, a(3)=6, a(4)=24,
 *            a(5)=120, a(6)=664, a(7)=3596, a(8)=19004
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const seq_t init_values[8] = {
    (seq_t)1ULL,
    (seq_t)2ULL,
    (seq_t)6ULL,
    (seq_t)24ULL,
    (seq_t)120ULL,
    (seq_t)664ULL,
    (seq_t)3596ULL,
    (seq_t)19004ULL
};

seq_t a(idx_t n)
{
    /* n is 1‑based per OEIS specification */
    if (n <= 8)
        return init_values[(size_t)(n - 1)];

    /* use a fixed-size circular buffer of the last 8 computed values */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init_values[i];

    /* compute terms from position 9 (index 8) up to n */
    for (idx_t i = 8; i < n; ++i) {
        /* indices relative to the current position i:
           a[i-1], a[i-2], ..., a[i-8] are stored in buf[(i-1)%8], etc. */
        seq_t v0 = buf[(i - 1) % 8];
        seq_t v1 = buf[(i - 2) % 8];
        seq_t v2 = buf[(i - 3) % 8];
        seq_t v3 = buf[(i - 4) % 8];
        seq_t v4 = buf[(i - 5) % 8];
        seq_t v5 = buf[(i - 6) % 8];
        seq_t v6 = buf[(i - 7) % 8];
        seq_t v7 = buf[(i - 8) % 8];

        /* Compute a[i] using only addition and subtraction,
           ensuring the intermediate result never underflows
           (all positive contributions are summed first). */
        seq_t term = 8 * v0;            /* 8*a(i-1) */
        term += 26 * v2;               /* +26*a(i-3) */
        term += 12 * v4;               /* +12*a(i-5) */
        term += 8 * v5;                /* + 8*a(i-6) */
        term += 8 * v7;                /* + 8*a(i-8) */
        term -= 19 * v1;               /* -19*a(i-2) */
        term -= 14 * v3;               /* -14*a(i-4) */
        term -= 20 * v6;               /* -20*a(i-7) */

        buf[i % 8] = term;
    }

    return buf[(n - 1) % 8];
}
