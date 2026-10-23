/*
 * A182420  a(0)=0, a(n) = (a(n-1) XOR n) * n.
 * https://oeis.org/A182420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n) = (a(n-1) XOR n) * n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = 0;
    for (idx_t i = 1; i <= n; ++i)
        a_val = (a_val ^ (seq_t)i) * (seq_t)i;
    return a_val;
}
