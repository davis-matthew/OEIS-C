/*
 * A070423  a(n) = 7^n mod 40.
 * https://oeis.org/A070423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 40
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 4:
       7^0 ≡ 1, 7^1 ≡ 7, 7^2 ≡ 9, 7^3 ≡ 23 (mod 40) */
    static const seq_t tbl[4] = {1, 7, 9, 23};
    return tbl[n & 3];   /* n % 4 without a division */
}
