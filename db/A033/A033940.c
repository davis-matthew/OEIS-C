/*
 * A033940  a(n) = 10^n mod 7.
 * https://oeis.org/A033940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n mod 7
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 6: 1, 3, 2, 6, 4, 5. */
seq_t a(idx_t n)
{
    static const seq_t tbl[6] = { 1, 3, 2, 6, 4, 5 };
    return tbl[n % 6];
}
