/*
 * A103623  a(n) = n^9 + n^8 + n^7 + n^6 + n^5 + n^4 + n^3 + n^2 + n + 1.
 * https://oeis.org/A103623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{9} n^k = n^9 + n^8 + n^7 + n^6 + n^5 + n^4 +
 *            n^3 + n^2 + n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Also a(n) = (n^10 - 1) / (n - 1)  with a(1) = 10 */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;
    seq_t sn = (seq_t)n;
    for (idx_t i = 0; i < 10; ++i)
        res = res * sn + (seq_t)1;
    return res;
}
