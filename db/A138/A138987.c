/*
 * A138987  a(n) = Frobenius number for 7 successive numbers = F(n+1, n+2,
 *          n+3, n+4, n+5, n+6, n+7).
 * https://oeis.org/A138987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n+1)*floor((n-1)/6)
 * Range:     u8 n <= 36, u16 n <= 624, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero or negative indices are not part of the OEIS sequence; 
       we simply return 0 for n == 0 to avoid underflow on n-1. */
    if (n == 0)
        return 0;

    idx_t q = (n - 1) / 6;          /* floor((n-1)/6) using unsigned division */

    seq_t n_s  = (seq_t)n;           /* promote n to seq_t */
    seq_t n1_s = n_s + 1;            /* (n+1) as seq_t */

    /* Compute a(n) = n + (n+1)*q */
    return n_s + n1_s * (seq_t)q;
}
