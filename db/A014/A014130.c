/*
 * A014130  ((n+3)!/6)*product( 2*k+1, k=0..n).
 * https://oeis.org/A014130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+3)!/6) * product_{k=0}^n (2*k+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;

    /* (n+3)!/6 = product of integers 4 through n+3 */
    if (n >= 1) {
        for (idx_t i = 4; i <= n + 3; ++i)
            res *= (seq_t)i;
    }

    /* Multiply by odd double factorial (2n+1)!! = product_{k=0}^n (2k+1) */
    for (idx_t k = 0; k <= n; ++k)
        res *= (seq_t)(2 * k + 1);

    return res;
}
