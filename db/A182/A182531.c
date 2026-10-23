/*
 * A182531  Extremal graph numbers for a triangle with an edge off it.
 * https://oeis.org/A182531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=3, and for n≥4, a(n)=⌊n^2/4⌋
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the special small cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* for n ≥ 4: floor(n^2/4).  Integer division truncates towards zero,
       which is equivalent to floor for non‑negative numbers. */
    return ((seq_t)n * (seq_t)n) / (seq_t)4;
}
