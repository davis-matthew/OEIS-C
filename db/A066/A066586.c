/*
 * A066586  Number of normal subgroups of the group of n X n signed
 *          permutations matrices (described in sequence A066051).
 * https://oeis.org/A066586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=6, a(3)=9, a(4)=11, a(n)=9 for all n≥5
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)6;
    if (n == (idx_t)3) return (seq_t)9;
    if (n == (idx_t)4) return (seq_t)11;
    return (seq_t)9;
}
