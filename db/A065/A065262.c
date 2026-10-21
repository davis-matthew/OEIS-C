/*
 * A065262  The nonpositive side (-1, -2, -3, ...) of the site swap sequence
 *          A065261. The bisection of odd terms of A065261.
 * https://oeis.org/A065262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n is even, a(n) = 2*n-1 if n is odd
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Offset 1: a(1)=1, a(2)=1, a(3)=5, a(4)=2, a(5)=9, a(6)=3, …) */
seq_t a(idx_t n)
{
    if (n & 1) {               /* odd n */
        return ((seq_t)n << 1) - 1;   /* 2*n - 1 */
    } else {                    /* even n */
        return (seq_t)n >> 1;          /* n/2 */
    }
}
