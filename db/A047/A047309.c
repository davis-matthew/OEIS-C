/*
 * A047309  Numbers that are congruent to {1, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A047309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*floor((n-1)/5) + {1,3,4,5,6}[(n-1)%5]
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int offset[5] = {1, 3, 4, 5, 6};

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1. Returning 0 for n==0 is harmless. */
    if (n == 0) {
        return 0;
    }
    idx_t q = (n - 1) / 5;        /* quotient of the 5‑block index  */
    idx_t r = (n - 1) % 5;        /* position inside the block       */
    seq_t res = (seq_t)q * 7 + offset[r];
    return res;
}
