/*
 * A017154  a(n) = (8*n + 7)^6.
 * https://oeis.org/A017154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^6
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 202, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (8*n + 7) in seq_t */
    seq_t base = (seq_t)8 * n + 7;

    /* Compute base^6 using exponentiation by squaring
       to avoid repeated multiplications and use only 3 mults */
    seq_t base2 = base * base;          /* base^2 */
    seq_t base4 = base2 * base2;        /* base^4 */
    seq_t result = base4 * base2;       /* base^4 * base^2 = base^6 */

    return result;
}
