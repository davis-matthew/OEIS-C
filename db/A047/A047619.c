/*
 * A047619  Numbers that are congruent to {1, 2, 5} mod 8.
 * https://oeis.org/A047619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/3) + [1,2,5][(n-1)%3]
 * Range:     u8 n <= 96, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset table for (n‑1)%3: 0 → 1, 1 → 2, 2 → 5 */
    static const seq_t OFF[3] = {(seq_t)1, (seq_t)2, (seq_t)5};
    idx_t k = n - 1;
    idx_t cycle = k / 3;
    idx_t r = k % 3;
    seq_t base = (seq_t)cycle * (seq_t)8;
    return base + OFF[r];
}
