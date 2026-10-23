/*
 * A154030  Sequence defined by a(2*n) = 2*(n^2 + 2*n) and a(2*n-1) =
 *          (2*n)!/n!.
 * https://oeis.org/A154030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 2*(n^2 + 2*n); a(2*n-1) = (2*n)!/n!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even indices: n = 2*k.  Return 2*(k^2 + 2*k). */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                /* k = n/2 */
        seq_t sk = (seq_t)k;
        seq_t res = 2 * (sk * sk + 2 * sk);
        return res;
    }
    /* Odd indices: n = 2*k-1  =>  k = (n+1)/2.
       Compute (k+1)*(k+2)*...*(2*k) which equals (2*k)!/k!. */
    idx_t k = (n + 1) >> 1;              /* k = (n+1)/2 */
    seq_t prod = 1;
    for (idx_t i = k + 1; i <= 2 * k; ++i) {
        prod *= (seq_t)i;
    }
    return prod;
}
