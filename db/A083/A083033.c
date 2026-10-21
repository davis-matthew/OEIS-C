/*
 * A083033  Numbers that are congruent to {0, 2, 3, 5, 7, 9, 10} mod 12.
 * https://oeis.org/A083033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n-1) mod 7] + 12 * floor((n-1)/7) where base =
 *            {0,2,3,5,7,9,10} corresponds to a(1)..a(7)
 * Range:     u8 n <= 150, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* off‑spec, just safe */
    static const seq_t base[7] = {0,2,3,5,7,9,10};
    idx_t q = (n - 1) / 7;          /* how many full blocks of 7 */
    idx_t r = (n - 1) % 7;          /* position inside the block */
    return base[r] + (seq_t)12 * (seq_t)q;
}
