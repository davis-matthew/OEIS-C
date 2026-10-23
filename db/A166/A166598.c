/*
 * A166598  a(n) = 5*n - a(n-1), with n>1, a(1)=5.
 * https://oeis.org/A166598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*floor((n+1)/2)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative recurrence: a(n) = 5*n - a(n-1),  a(1) = 5 */

seq_t a(idx_t n)
{
    /* Compute floor((n+1)/2) without overflow:
       (n >> 1) gives floor(n/2), (n & 1) adds 1 only when n is odd. */
    seq_t half_plus = ((seq_t)(n >> 1)) + (seq_t)(n & 1);
    return half_plus * (seq_t)5;
}
