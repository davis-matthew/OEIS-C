/*
 * A226801  Column 6 of array in A226513.
 * https://oeis.org/A226801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n^5 + 35*n^4 + 430*n^3 + 2320*n^2 + 5525*n +
 *            4683)
 * Range:     u8 none, u16 n <= 1, u32 n <= 34, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t first to avoid any implicit narrowing. */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n in seq_t. */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */

    /* Evaluate the degree‑5 polynomial in the parentheses. */
    seq_t poly = n5
               + (seq_t)35  * n4
               + (seq_t)430  * n3
               + (seq_t)2320 * n2
               + (seq_t)5525 * nn
               + (seq_t)4683;

    /* Multiply by (n + 1). */
    return (nn + 1) * poly;
}
