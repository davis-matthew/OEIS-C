/*
 * A093178  If n is even then 1, otherwise n.
 * https://oeis.org/A093178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9408, u32 n <= 9408, u64 n <= 9408,
 *            u128 n <= 9408, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if n is even, otherwise a(n) = n.
 * This follows from the simple closed‑form
 *   a(n) = (1/2)*(1 + n + (1 - n)*(-1)^n)
 * or equivalently from a(n) = n^n mod (n+1) for n>0.
 */
seq_t a(idx_t n)
{
    return (seq_t)((n & 1U) ? n : 1);
}
