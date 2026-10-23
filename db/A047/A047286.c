/*
 * A047286  Numbers that are congruent to {1, 2, 3, 6} mod 7.
 * https://oeis.org/A047286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/4) + offset[(n-1) mod 4] where offset =
 *            {1,2,3,6}
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n<1, return 0 */
    static const int offs[4] = {1, 2, 3, 6};
    idx_t k = (n - 1) / 4;       /* number of complete blocks of 7 */
    idx_t t = (n - 1) % 4;       /* position within the block (0‑3) */
    seq_t res = (seq_t)7 * (seq_t)k;
    res += offs[t];
    return res;
}
