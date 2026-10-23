/*
 * A140670  a(n) = 1 if n is odd; otherwise, a(n) = 2^k - 1 where 2^k is the
 *          largest power of 2 that divides n.
 * https://oeis.org/A140670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is odd; otherwise 2^k - 1 where 2^k is the largest
 *            power of 2 that divides n
 * Range:     u8 n <= 511, u16 n <= 9678, u32 n <= 9678, u64 n <= 9678,
 *            u128 n <= 9678, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for n=0, though n≥1 */
    if (n & (idx_t)1) return (seq_t)1;    /* odd n → 1 */
    idx_t k = 0;
    idx_t m = n;
    while ((m & (idx_t)1) == 0) {         /* count factors of 2 */
        m >>= 1;
        ++k;
    }
    return ((seq_t)1 << k) - (seq_t)1;    /* 2^k - 1 */
}
