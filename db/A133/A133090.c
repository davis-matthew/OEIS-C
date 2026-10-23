/*
 * A133090  A133081 * [1,2,3,...].
 * https://oeis.org/A133090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)*n - (n/2)*(-1)^n - 1
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    if ((n & (idx_t)1) == 0) {          /* n even: a(n) = n - 1 */
        return (seq_t)(n - 1);
    } else {                            /* n odd:  a(n) = 2*n - 1 */
        return (seq_t)(((seq_t)n * 2ULL) - 1);
    }
}
