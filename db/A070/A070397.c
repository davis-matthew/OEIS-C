/*
 * A070397  a(n) = 6^n mod 25.
 * https://oeis.org/A070397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period 5: 6^5 ≡ 1 (mod 25) */
    static const seq_t tbl[5] = {1, 6, 11, 16, 21};
    return tbl[n % 5];
}
