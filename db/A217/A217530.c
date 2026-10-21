/*
 * A217530  a(n) = n^4/2-5*n^3/2+21*n-30.
 * https://oeis.org/A217530
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4/2-5*n^3/2+21*n-30
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 305, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n < 2 the value is undefined for the OEIS sequence, but
       we simply return 0 for safety. */
    if (n < 2)
        return 0;

    /* Compute each term using seq_t to keep the calculation exact. */
    seq_t n2 = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t n3 = n2 * (seq_t)n;                /* n^3 */
    seq_t n4 = n3 * (seq_t)n;                /* n^4 */

    seq_t term1 = n4 / 2;                    /* n^4/2 */
    seq_t term2 = (5 * n3) / 2;              /* 5*n^3/2 */

    return term1 - term2 + 21 * (seq_t)n - 30;
}
