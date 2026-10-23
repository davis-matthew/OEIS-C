/*
 * A047328  Numbers that are congruent to {0, 3, 5, 6} mod 7.
 * https://oeis.org/A047328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * ((n-1)/4) + rem[(n-1)%4], where rem = {0, 3, 5, 6}
 *            (acceptable residues modulo 7)
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range – harmless */
    seq_t q = (seq_t)((n - 1) / 4);       /* number of complete 7‑blocks */
    int    r = (int)((n - 1) % 4);        /* position inside the block   */
    static const seq_t rem[4] = {0, 3, 5, 6};
    return 7 * q + rem[r];
}
