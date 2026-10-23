/*
 * A126480  Number of base 12 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n+5) = 8*a(n+4) - 8*a(n+3) - 18*a(n+2) + 10*a(n+1) + 6*a(n)
 *            (for n >= 1), with the initial values: a(0)=1, a(1)=12, a(2)=72,
 *            a(3)=448, a(4)=2816, a(5)=17780
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* buffer holds the last six values a(k) with k mod 6 mapping */
    static seq_t buf[6] = {
        (seq_t)1,          /* a(0) */
        (seq_t)12,         /* a(1) */
        (seq_t)72,         /* a(2) */
        (seq_t)448,        /* a(3) */
        (seq_t)2816,       /* a(4) */
        (seq_t)17780       /* a(5) */
    };
    static idx_t max_n = 5;   /* last index that has been computed */
    static bool init = false;/* ensures one‑time initialization */

    (void)init; /* silence unused variable warning if compiler is picky */

    if (n <= 5)
        return buf[n % 6];

    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* indices of needed previous terms */
        idx_t i1 = (i - 1) % 6; /* a(i-1) */
        idx_t i2 = (i - 2) % 6; /* a(i-2) */
        idx_t i3 = (i - 3) % 6; /* a(i-3) */
        idx_t i4 = (i - 4) % 6; /* a(i-4) */
        idx_t i5 = (i - 5) % 6; /* a(i-5) */

        /* s = 8*a(i-1) + 10*a(i-4) + 6*a(i-5) */
        seq_t s = (seq_t)8 * buf[i1] + (seq_t)10 * buf[i4] + (seq_t)6 * buf[i5];
        /* t = 8*a(i-2) + 18*a(i-3) */
        seq_t t = (seq_t)8 * buf[i2] + (seq_t)18 * buf[i3];

        /* a(i) = s - t ; the recurrence guarantees s >= t */
        buf[i % 6] = s - t;
    }
    max_n = n;
    return buf[n % 6];
}
