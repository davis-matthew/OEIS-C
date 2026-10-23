/*
 * A182687  a(n) = the smallest 1-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the smallest 1‑digit number with exactly n divisors.
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=6; a(n)=0 otherwise
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)4;
    if (n == (idx_t)4) return (seq_t)6;
    return (seq_t)0;
}
