/*
 * A116842  Number of permutations of length n which avoid the patterns 1234,
 *          2143, 3421.
 * https://oeis.org/A116842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41*n^2 - 339*n + 739 for n >= 5 a(1) = 1, a(2) = 2, a(3)
 *            = 6, a(4) = 21 (OEIS A116842)
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset and base cases */
    if (n == 0) return 0;                 /* undefined, but safe fallback */
    if (n <= 4) {
        static const seq_t base_vals[4] = { (seq_t)1,
                                            (seq_t)2,
                                            (seq_t)6,
                                            (seq_t)21 };
        return base_vals[n - 1];
    }

    /* compute the quadratic formula safely in unsigned arithmetic */
    seq_t t  = (seq_t)n;          /* n as seq_t */
    seq_t sq = t * t;             /* n^2 */
    seq_t tmp = (seq_t)41 * sq + (seq_t)739;   /* 41*n^2 + 739 */
    return tmp - (seq_t)339 * t;  /* subtract 339*n; guaranteed non‑negative */
}
