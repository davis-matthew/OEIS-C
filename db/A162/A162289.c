/*
 * A162289  a(n) = 1 if n is relatively prime to 30 else 0.
 * https://oeis.org/A162289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is relatively prime to 30 else 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9361 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* numbers divisible by 2, 3, or 5 are not coprime to 30 */
    if ((n % 2) == 0) return (seq_t)0;
    if ((n % 3) == 0) return (seq_t)0;
    if ((n % 5) == 0) return (seq_t)0;
    return (seq_t)1;
}
