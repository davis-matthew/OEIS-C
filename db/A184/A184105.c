/*
 * A184105  n-1+ceiling((n/5)^2); complement of A184104.
 * https://oeis.org/A184105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 + ceil((n/5)^2) = n-1 + (n*n + 24)/25
 * Range:     u8 n <= 15, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t prod = tn * tn;
    seq_t add = (prod + 24) / 25;
    return tn - 1 + add;
}
