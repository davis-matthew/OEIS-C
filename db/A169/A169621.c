/*
 * A169621  Hankel transform of quintuple factorial numbers A047055.
 * https://oeis.org/A169621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{k=0..n} (5*(k+1)*(5*k+2))^(n-k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t prod   = 1;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t k1   = (seq_t)(k + 1);
        seq_t term1 = 5 * k1;                 /* 5*(k+1) */
        seq_t term2 = ((seq_t)k * 5) + 2;      /* 5*k + 2 */
        seq_t c     = term1 * term2;          /* c_k = 5*(k+1)*(5*k+2) */
        prod *= c;                            /* product up to k */
        result *= prod;                       /* update a(k+1) */
    }
    return result;
}
