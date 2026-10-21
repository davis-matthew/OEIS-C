/*
 * A053104  a(n) = ((7*n+8)(!^7))/8, related to A045754 ((7*n+1)(!^7) sept-,
 *          or 7-factorials).
 * https://oeis.org/A053104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{k=1..n} (7*k + 8), with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        res = res * ((seq_t)(7 * k + 8));
    }
    return res;
}
