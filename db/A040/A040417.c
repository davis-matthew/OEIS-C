/*
 * A040417  Continued fraction for sqrt(438).
 * https://oeis.org/A040417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 20; for n > 0 let r = (n-1) mod 4 then a(n) = 1 if r=0 or
 *            2, 12 if r=1, 40 if r=3. This repeats with period 4 starting at
 *            n=1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)20;
    idx_t r = (n - 1u) & 3u;           /* (n-1) % 4 */
    if (r == 0u) return (seq_t)1;
    if (r == 1u) return (seq_t)12;
    if (r == 2u) return (seq_t)1;
    return (seq_t)40;
}
