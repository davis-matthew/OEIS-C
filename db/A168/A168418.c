/*
 * A168418  a(n) = 9*n - a(n-1) - 8 with n>1, a(1)=5.
 * https://oeis.org/A168418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n - a(n-1) - 8, a(1)=5 Closed form: a(n) = 5 +
 *            9*floor((n-1)/2)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* undefined for n < 1 */
    seq_t N     = (seq_t)n;             /* cast index to sequence type */
    seq_t shift = (N - 1) / 2;          /* floor((n-1)/2) */
    return (seq_t)5 + (seq_t)9 * shift;
}
