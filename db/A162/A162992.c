/*
 * A162992  The second right hand column of triangle A162990.
 * https://oeis.org/A162992
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)!/(n-1))^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n < 2 are not defined, but return 0 safely */
    if (n < 2) return 0;

    /* compute (n-2)! */
    seq_t prod = 1;
    for (idx_t i = 1; i <= n - 2; ++i) {
        prod *= (seq_t)i;
    }

    /* multiply by n and (n+1) */
    prod *= (seq_t)n;
    prod *= ((seq_t)n + 1);

    /* square the result */
    prod = prod * prod;
    return prod;
}
