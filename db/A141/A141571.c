/*
 * A141571  Decimal expansion of 11999/99900.
 * https://oeis.org/A141571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = 1, 2, 0, and for n ≥ 3 a(n) = 1 when (n‑3) mod 3 = 0 or
 *            1, otherwise a(n) = 0. This corresponds to the repeating block
 *            1, 1, 0 after the initial digits 1, 2, 0 of the decimal
 *            expansion of 11999/99900
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   return (seq_t)1;
    if (n == 1)   return (seq_t)2;
    if (n == 2)   return (seq_t)0;

    /* n ≥ 3 use pattern of length 3 (1, 1, 0) */
    idx_t r = (n - 3) % 3;
    return (r == 0 || r == 1) ? (seq_t)1 : (seq_t)0;
}
