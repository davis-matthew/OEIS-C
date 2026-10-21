/*
 * A204439  Symmetric matrix: f(i,j)=((i+j+2)^2 mod 3), by (constant)
 *          antidiagonals.
 * https://oeis.org/A204439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if d % 3 != 0 else 0, where d = smallest integer with
 *            d*(d+1)/2 >= n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Values come from the constant anti-diagonals of the matrix ((i+j+2)^2 mod 3)) */
seq_t a(idx_t n)
{
    /* Find the diagonal index d: minimal d such that d*(d+1)/2 >= n */
    idx_t low = 1;
    idx_t high = 1;
    /* Grow high until the condition holds.  The product is evaluated in 128 bits to avoid overflow. */
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t d = low;
    /* On each anti-diagonal all entries are equal: (-1)^(d%3) essentially, but here 1 for d%3≠0, 0 otherwise. */
    seq_t val = (seq_t)((d % 3 == 0) ? 0 : 1);
    return val;
}
