/*
 * A081753  a(n) = floor(n/12) if n == 2 (mod 12); a(n) = floor(n/12) + 1
 *          otherwise.
 * https://oeis.org/A081753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/12) if n == 2 (mod 12); otherwise floor(n/12) + 1
 * Range:     u8 n <= 661, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t r = n % 12;             /* 0 ≤ r < 12 */
    const seq_t q = ((seq_t)n) / 12;    /* floor(n/12) as seq_t */

    return (r == 2) ? q : q + 1;
}
