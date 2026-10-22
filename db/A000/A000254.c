/*
 * A000254  Unsigned Stirling numbers of first kind, s(n+1,2): a(n+1) =
 *          (n+1)*a(n) + n!.
 * https://oeis.org/A000254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*a(n-1) + (n-1)!, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    seq_t f = 1; /* (k-1)! */
    idx_t k;
    for (k = 1; k <= n; k++) {
        r = r * (seq_t)k + f;
        f = f * (seq_t)k;
    }
    return r;
}
