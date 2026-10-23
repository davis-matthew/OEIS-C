/*
 * A017046  a(n) = (7*n + 5)^6.
 * https://oeis.org/A017046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 231, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)7 + (seq_t)5;
    seq_t r = base * base;   /* base^2 */
    r *= base;               /* base^3 */
    r *= r;                  /* (base^3)^2 = base^6 */
    return r;
}
