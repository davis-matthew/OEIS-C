/*
 * A125915  Sprague-Grundy values for octal game .147.
 * https://oeis.org/A125915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sprague‑Grundy values for octal game .147. a(1) = 1 a(2) = 0 For
 *            n >= 3: let r = n mod 8 if r == 0 or r == 7: a(n) = 4 else if r
 *            <= 3: a(n) = 1 else a(n) = 2 (The sequence is eventually
 *            periodic with period 8.)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    idx_t r = n % 8;
    if (r == 0 || r == 7) return (seq_t)4;
    if (r <= 3)            return (seq_t)1;
    return (seq_t)2;
}
