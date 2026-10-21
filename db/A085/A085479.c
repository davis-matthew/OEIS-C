/*
 * A085479  Product of three solutions of the Diophantine equation x^3 - y^3 =
 *          z^2.
 * https://oeis.org/A085479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 728 * n^7
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 220, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A085479.
 * The formula is a(n) = 728 * n^7.
 * This computation uses only integer multiplication, compatible with
 * arbitrary-precision seq_t and up to 128-bit integer types. */
seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* Convert index to sequence type */
    seq_t prod = nn;                /* start with n^1 */
    prod *= nn;                     /* n^2 */
    prod *= nn;                     /* n^3 */
    prod *= nn;                     /* n^4 */
    prod *= nn;                     /* n^5 */
    prod *= nn;                     /* n^6 */
    prod *= nn;                     /* n^7 */
    prod *= (seq_t)728;              /* multiply by 728 */
    return prod;
}
