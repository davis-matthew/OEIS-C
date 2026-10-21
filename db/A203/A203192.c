/*
 * A203192  (n-1)-st elementary symmetric function of the first n terms of
 *          (1,2,1,4,1,6,1,8,...)=(A124625 for n>1).
 * https://oeis.org/A203192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-O)*2^k * k! + Σ_{i=1}^{k} 2^{k-1} * k! / i where k =
 *            floor(n/2), O = n - k (number of odd indices). This follows from
 *            a(n) = Σ_{i=1}^{n} Π_{j≠i} b_j with b_j = 1 for odd j, b_j = j
 *            for even j
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* k = floor(n/2), number of even indices */
    idx_t k = n / 2;
    /* O = number of odd indices = n - k */
    idx_t o = n - k;

    /* Compute k! */
    seq_t prod = 1;
    for (idx_t i = 1; i <= k; ++i)
        prod *= (seq_t)i;

    /* Compute 2^k */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2 <<= 1;

    /* First part: (n - k) * 2^k * k! */
    seq_t result = pow2 * prod * (seq_t)o;

    /* Second part: Σ_{i=1}^{k} 2^{k-1} * k! / i */
    seq_t pow2_half = pow2 >> 1;              /* 2^{k-1} */
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t term = (pow2_half * prod) / (seq_t)i;
        result += term;
    }

    return result;
}
