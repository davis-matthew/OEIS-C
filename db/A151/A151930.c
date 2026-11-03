/*
 * A151930  First differences of A001316.
 * https://oeis.org/A151930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2 - 2^p) * 2^popcount( ((n+1) >> p - 1) / 2 ), where p =
 *            exponent of the largest power of 2 dividing n+1
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 94 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return 2^popcount(n).  This is the Stern value S(n). */
static seq_t stern_from_idx(idx_t n)
{
    seq_t v = (seq_t)1;
    while (n) {
        if (n & 1) v *= (seq_t)2;   /* multiply by 2 for every set bit */
        n >>= 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    idx_t t = n + 1;          /* t = n+1 */
    unsigned int p = 0;       /* trailing zero count of t */
    while ((t & 1) == 0) {
        t >>= 1;
        ++p;
    }

    /* t is now odd; compute n0 = (t-1)/2 */
    idx_t n0 = (t - 1) >> 1;

    seq_t s = stern_from_idx(n0);

    seq_t factor = (seq_t)2 - ((seq_t)1 << p);
    return factor * s;
}
