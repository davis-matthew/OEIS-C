/*
 * A048724  Write n and 2n in binary and add them mod 2.
 * https://oeis.org/A048724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, look, easy, base
 * Formula:   a(n) = n XOR (n << 1) (carry‑less multiplication by 3)
 * Range:     u8 n <= 127, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n ^ ((seq_t)n << 1);
}
