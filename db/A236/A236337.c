/*
 * A236337  Expansion of (2 - x) / ((1 - x)^2 * (1 - x^3)) in powers of x.
 * https://oeis.org/A236337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = (n+1)*(3*n+4)/2 a(3*n+1) = (n+1)*(3*n+6)/2 a(3*n+2) =
 *            (n+1)*(3*n+8)/2 (All divisions are exact.)
 * Range:     u8 n <= 35, u16 n <= 623, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the arithmetic type seq_t to avoid overflows */
    seq_t n_seq = (seq_t)n;
    /* Determine n modulo 3 and the quotient */
    seq_t k = n_seq / 3;          /* k = floor(n/3)  */
    seq_t r = n_seq % 3;          /* r = n mod 3     */

    if (r == 0) {
        /* a(3*k) */
        return ((k + 1) * (3*k + 4)) / 2;
    } else if (r == 1) {
        /* a(3*k+1) */
        return ((k + 1) * (3*k + 6)) / 2;
    } else {
        /* a(3*k+2) */
        return ((k + 1) * (3*k + 8)) / 2;
    }
}
