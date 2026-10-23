/*
 * A047279  Numbers that are congruent to {0, 1, 2, 6} mod 7.
 * https://oeis.org/A047279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/4) + [0,1,2,6][(n-1)%4] (a(1)=0)
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = (n - 1) / 4;                 /* number of complete 7‑number blocks   */
    idx_t r = (n - 1) % 4;                 /* position inside the current block    */
    static const seq_t offset[4] = {0, 1, 2, 6};
    return 7 * ((seq_t)q) + offset[(unsigned int)r];
}
