/*
 * A100067  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*2^(n-2*k).
 * https://oeis.org/A100067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*2^(n-2*k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t c = 1;                 /* current binomial C(n,k)  */
    idx_t maxk = n / 2;          /* floor(n/2)                */

    /* compute 2^n in seq_t */
    seq_t pow2_n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2_n <<= 1;            /* shift left by 1 (×2)     */

    seq_t q = pow2_n;            /* 2^(n-2k) term            */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* add current term */
        result += c * q;

        /* prepare next binomial and power term if needed */
        if (k < maxk)
        {
            /* C(n,k+1) = C(n,k) * (n-k)/(k+1)  */
            c = (c * (seq_t)(n - k)) / (k + 1);
            /* 2^(n-2(k+1)) = 2^(n-2k-2) = 2^(n-2k)/4 */
            q >>= 2;               /* equivalent to q = q / 4 */
        }
    }

    return result;
}
