/*
 * A040252  Continued fraction for sqrt(269).
 * https://oeis.org/A040252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = floor(sqrt(269)) = 16; for n ≥ 1, a(n) = {2, 2, 32}
 *            repeated, i.e. a(n) = 2 if (n‑1) mod 3 == 0 or 1; a(n) = 32 if
 *            (n‑1) mod 3 == 2
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;
    idx_t r = (n - 1) % 3;
    if (r == 0 || r == 1)
        return (seq_t)2;
    return (seq_t)32;   /* r == 2 */
}
