/*
 * A011672  Expansion of reciprocal of x^6+x^3+1 (mod 2).
 * https://oeis.org/A011672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A011672: Expansion of reciprocal of x^6 + x^3 + 1 (mod 2).
   The sequence is periodic of period 9 with pattern
       0, 0, 0, 0, 0, 1, 0, 0, 1
   Hence a(n) = 1 if n mod 9 equals 5 or 8; otherwise a(n) = 0. */

seq_t a(idx_t n)
{
    idx_t r = n % 9;
    return (seq_t)((r == 5 || r == 8) ? 1 : 0);
}
