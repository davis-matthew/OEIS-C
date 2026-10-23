/*
 * A146763  Rank of terms ending in 0 in A061039.
 * https://oeis.org/A146763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*n if n is even; a(n) = 5*n-1 if n is odd
 * Range:     u8 n <= 51, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)5 * (seq_t)n;
    if (n & 1ULL) {
        res -= (seq_t)1;
    }
    return res;
}
