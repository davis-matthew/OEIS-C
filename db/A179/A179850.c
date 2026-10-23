/*
 * A179850  Characteristic function of numbers that are congruent to {0, 1, 3,
 *          4} mod 5.
 * https://oeis.org/A179850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n mod 5 ≠ 2, else 0
 * Range:     u8 n <= 4999, u16 n <= 4999, u32 n <= 4999, u64 n <= 4999,
 *            u128 n <= 4999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is zero‑based; the sequence is periodic mod 5 with only the residue 2 omitted */
    return (n % 5 == 2) ? (seq_t)0 : (seq_t)1;
}
