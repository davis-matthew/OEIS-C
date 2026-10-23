/*
 * A047514  Numbers that are congruent to {3, 4, 6, 7} mod 8.
 * https://oeis.org/A047514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 8 for n > 4 * Initial terms (offset 1): * a(1) =
 *            3, a(2) = 4, a(3) = 6, a(4) = 7
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 0 – return 0 */
    const seq_t base[4] = { (seq_t)3, (seq_t)4, (seq_t)6, (seq_t)7 };
    idx_t k = (n - 1) / 4;         /* number of times 8 has been added   */
    idx_t r = (n - 1) % 4;         /* residue modulo 4 (0..3)            */
    seq_t inc = (seq_t)8 * (seq_t)k;
    return base[r] + inc;
}
