/*
 * A056189  a(n) = 2^n - A056188(n).
 * https://oeis.org/A056189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - \sum_{k=1}^{n-1} C(n,k) if gcd(n,k)=1, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* for size_t, abort() */
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t, idx_t) */
#include "../utils/is_prime.h"   /* declares bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Fast case: primes have a(n)=2 */
    if (is_prime(n))
        return (seq_t)2;

    /* Compute 2^n by repeated multiplication */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 = pow2 * (seq_t)2;

    /* Sum binomial coefficients with coprime k */
    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)1;   /* C(n,0) */

    for (idx_t k = 1; k < n; ++k) {
        /* Update binom to C(n,k) */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        if (gcd((seq_t)n, (seq_t)k) == (seq_t)1)
            sum += binom;
    }

    return pow2 - sum;
}
