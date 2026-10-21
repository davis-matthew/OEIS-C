/*
 * A072946  Coefficient of the highest power of q in the expansion of nu(0)=1,
 *          nu(1)=b and for n>=2, nu(n)=b*nu(n-1)+lambda*(n-1)_q*nu(n-2) with
 *          (b,lambda)=(2,2), where (n)_q=(1+q+...+q^(n-1)) and q is a root of
 *          unity.
 * https://oeis.org/A072946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(2k)=3·2^k; a(2k+1)=2^(k+1)
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* even indices: a(2k) = 3 * 2^k */
    if ((n & 1U) == 0) {
        idx_t k = n >> 1;   /* k = n / 2 */
        return ((seq_t)3) << k;
    }

    /* odd indices: a(2k+1) = 2^(k+1)  with  k = (n-1)/2  ⇒  (n+1)/2 */
    idx_t k = (n + 1) >> 1;   /* k = (n+1)/2 = (n-1)/2 + 1 */
    return ((seq_t)1) << k;
}
