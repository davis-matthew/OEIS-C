/*
 * A070370  a(n) = 5^n mod 16.
 * https://oeis.org/A070370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 16
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 4: 1, 5, 9, 13. */
    static const seq_t tbl[4] = { (seq_t)1, (seq_t)5, (seq_t)9, (seq_t)13 };
    return tbl[n & 3];   /* n mod 4 */
}
