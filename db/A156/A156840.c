/*
 * A156840  Numbers k >= 1 such that k^2 == 1 (mod 900).
 * https://oeis.org/A156840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n%4) + 450*(n/4), where a(0)=1, a(1)=199, a(2)=251,
 *            a(3)=449
 * Range:     u8 n <= 2, u16 n <= 582, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[4] = { 1, 199, 251, 449 };
    idx_t q = n / 4;          /* number of complete 4‑term blocks */
    idx_t r = n % 4;          /* position inside the block     */
    return base[r] + (seq_t)q * 450;
}
