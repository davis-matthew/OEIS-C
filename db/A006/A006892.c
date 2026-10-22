/*
 * A006892  Representation as a sum of squares requires n squares with greedy
 *          algorithm.
 * https://oeis.org/A006892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n <= 3; for n > 3, a(n) = a(n-1) + ((a(n-1)+1)/2)^2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        return (seq_t)n;
    }
    
    seq_t val = (seq_t)3;
    for (idx_t i = 4; i <= n; i++) {
        seq_t h = (val + 1) / 2;
        val = val + h * h;
    }
    return val;
}
