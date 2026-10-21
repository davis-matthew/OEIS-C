/*
 * A070410  a(n) = 7^n mod 25.
 * https://oeis.org/A070410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic with period 4: 1, 7, 24, 18 */

seq_t a(idx_t n)
{
    static const seq_t tbl[4] = {1, 7, 24, 18};
    return tbl[n % 4];
}
