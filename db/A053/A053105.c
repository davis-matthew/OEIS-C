/*
 * A053105  a(n) = ((7*n+9)(!^7))/9(!^7), related to A034829 (((7*n+2)(!^7))/2
 *          sept-, or 7-factorials).
 * https://oeis.org/A053105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{k=1..n} (7*k+9), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute term (7*k+9) in seq_t to avoid overflow */
        seq_t term = ((seq_t)k * 7) + 9;
        res *= term;
    }
    return res;
}
