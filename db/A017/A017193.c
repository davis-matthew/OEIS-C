/*
 * A017193  a(n) = (9*n + 2)^9.
 * https://oeis.org/A017193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 15, u128 n <= 2123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 9*n + 2 as seq_t */
    seq_t base = (seq_t)9 * (seq_t)n + (seq_t)2;

    /* Compute base^9 using a few multiplications.
       base^9 = (base^3)^3 = (base^6) * base^3 and
       base^6 = (base^3)^2 */
    seq_t base2 = base * base;          /* base^2   */
    seq_t base3 = base2 * base;         /* base^3   */
    seq_t base6 = base3 * base3;        /* base^6   */
    return base6 * base3;               /* base^9   */
}
