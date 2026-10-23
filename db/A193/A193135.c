/*
 * A193135  Numbers of spanning trees of the halved cube graphs.
 * https://oeis.org/A193135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (1/2^(n-1)) * ∏_{k=1}^{n-1} (2k)^{C(n-1,k)} *
 *            ∏_{k=2}^{n-1} k^{C(n-1,k-1)}
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the utility providing the big_pow function */
#include "../utils/big_pow.h"

/* Helper: binomial coefficient for unsigned 64 bit integers.
   The result is assumed to fit in idx_t. */
static inline idx_t binom_uint64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = r * (n - k + i) / i;
    }
    return r;
}

/* Main function */
seq_t a(idx_t n)
{
    idx_t A = n - 1;          /* A = n-1 */
    seq_t result = 1;

    /* First product: ∏_{k=1}^{A} (2k)^{C(A,k)} */
    for (idx_t k = 1; k <= A; ++k)
    {
        idx_t exp = binom_uint64(A, k);
        seq_t base = (seq_t)(2 * k);
        result *= big_pow(base, exp);
    }

    /* Second product: ∏_{k=2}^{A} k^{C(A,k-1)} */
    for (idx_t k = 2; k <= A; ++k)
    {
        idx_t exp = binom_uint64(A, k - 1);
        seq_t base = (seq_t)k;
        result *= big_pow(base, exp);
    }

    /* Divide by 2^(n-1) = 2^A */
    seq_t pow2A = big_pow((seq_t)2, A);
    result /= pow2A;

    return result;
}
