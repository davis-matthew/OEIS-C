/*
 * A017613  a(n) = (12*n + 7)^9.
 * https://oeis.org/A017613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 7)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10, u128 n <= 1592,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t early to avoid unintended promotion */
    seq_t base = ((seq_t)n * 12u) + 7u;

    /* Compute base^9 using exponentiation by squaring */
    seq_t b2 = base * base;   /* base^2 */
    seq_t b4 = b2 * b2;       /* base^4 */
    seq_t b8 = b4 * b4;       /* base^8 */
    return b8 * base;         /* base^9 */
}
