/*
 * A166517  a(n) = (3 + 5*(-1)^n + 6*n)/4.
 * https://oeis.org/A166517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 5*(-1)^n + 6*n)/4
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For even n: a(n) = 2 + 3*(n/2)     (n = 2k)
       For odd  n: a(n) = 1 + 3*((n-1)/2) (n = 2k+1)   */
    if ((n & 1ULL) == 0) {
        return 2 + 3 * ((seq_t)n / 2);
    } else {
        return 1 + 3 * ((seq_t)(n - 1) / 2);
    }
}
