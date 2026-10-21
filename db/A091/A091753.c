/*
 * A091753  Fourth column (m=5) of array A091752 ((-1,2)Stirling2) divided by
 *          -6.
 * https://oeis.org/A091753
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)*(n-2)/4) * ∏_{i=1}^{n-2} ((3*i-1)*(3*i-2))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* value undefined for n < 3 */

    idx_t k = n - 2;                     /* number of terms in the product */
    seq_t prod = 1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t t1 = (seq_t)(3*i - 1);     /* (3i-1) */
        seq_t t2 = (seq_t)(3*i - 2);     /* (3i-2) */
        prod *= t1;
        prod *= t2;
    }

    seq_t result = prod;
    result *= (seq_t)(n - 1);
    result *= (seq_t)(n - 2);
    result /= (seq_t)4;                  /* division is exact */

    return result;
}
