/*
 * A060106  Numbers that are congruent to {1, 4, 6, 9, 11} mod 12. The ebony
 *          keys on a piano, starting with A0 = the 0th key.
 * https://oeis.org/A060106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12 * floor((n-1)/5) + d[(n-1)%5] where d = {1, 4, 6, 9,
 *            11}. This gives the numbers congruent to 1, 4, 6, 9, or 11
 *            modulo 12, starting with 1 at n = 1
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset array for the five residue classes modulo 12 */
    static const seq_t offset[5] = { (seq_t)1, (seq_t)4, (seq_t)6, (seq_t)9, (seq_t)11 };

    /* 0‑based index */
    idx_t k = n - 1;
    idx_t q = k / 5;          /* how many complete periods of 5 */
    idx_t r = k % 5;          /* position in current period */

    /* compute the value safely using seq_t arithmetic */
    seq_t base = (seq_t)12 * (seq_t)q;
    return base + offset[r];
}
