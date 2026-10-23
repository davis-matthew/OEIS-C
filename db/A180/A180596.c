/*
 * A180596  Digital root of 6n.
 * https://oeis.org/A180596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digital_root(6*n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t n9 = (seq_t)(n % 9);
    seq_t prod = (seq_t)6 * n9 % 9;
    return (prod == 0) ? (seq_t)9 : prod;
}
