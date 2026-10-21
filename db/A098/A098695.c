/*
 * A098695  a(n) = 2^(n(n-1)/2) * Product_{k=1..n} k!.
 * https://oeis.org/A098695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n(n-1)/2) * Product_{k=1..n} k! (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t fact   = (seq_t)1;
    seq_t pow2   = (seq_t)1;            /* holds 2^(k-1) inside the loop */

    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;               /* update k! */
        result *= fact;                 /* multiply by k!    */
        result *= pow2;                 /* multiply by 2^(k-1) */
        pow2  *= (seq_t)2;              /* prepare 2^k for next iteration */
    }

    return result;
}
