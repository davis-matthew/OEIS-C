/*
 * A201271  Number of n X 2 0..2 arrays with every row and column
 *          nondecreasing rightwards and downwards, and the number of
 *          instances of each value within one of each other.
 * https://oeis.org/A201271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A201271
   Formula:  a(n) =  (n mod 3 == 0) ? (n/3 + 1)^2
                      : (n mod 3 == 1) ? 3 * ((n+2)/3)^2
                      : 3 * ((n+1)/3)^2 + 2 * ((n+1)/3)   */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                 /* cast index to seq_t */
    seq_t mod3 = N % 3;

    if (mod3 == 0) {
        seq_t r = N / 3;                /* r = n/3 */
        return (r + 1) * (r + 1);       /* (r+1)^2 */
    } else if (mod3 == 1) {
        seq_t p = (N + 2) / 3;          /* p = (n+2)/3 */
        return 3 * p * p;               /* 3*p^2 */
    } else { /* mod3 == 2 */
        seq_t q = (N + 1) / 3;          /* q = (n+1)/3 */
        return 3 * q * q + 2 * q;       /* 3*q^2 + 2*q */
    }
}
