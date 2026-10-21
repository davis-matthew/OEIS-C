/*
 * A214865  n such that n XOR 9 = n - 9.
 * https://oeis.org/A214865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n) + 16, a(1)=9, a(2)=11, a(3)=13, a(4)=15
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t start[4] = {9, 11, 13, 15};
    if (n == 0) return 0;   /* outside offset – guard against n=0 */
    idx_t k = n - 1;         /* zero‑based index */
    idx_t r = k & 3;         /* k % 4 */
    idx_t m = k >> 2;        /* k / 4, integer division by 4 */
    seq_t res = start[r];
    res += (seq_t)m * (seq_t)16;
    return res;
}
