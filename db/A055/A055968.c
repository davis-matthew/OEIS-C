/*
 * A055968  Product_{i=1..n} (i-1)!*(i+2*n-1)!/(i+n-1)!.
 * https://oeis.org/A055968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1..n} (i-1)! * (i+2n-1)! / (i+n-1)!
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   exp_in_product_range, sum_floor_range, sum_floor_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/exp_in_product_range.h"
#include "../utils/sum_floor_range.h"
#include "../utils/sum_floor_upto.h"

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1) res *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    idx_t limit = 3 * n;                 /* largest factorial needed   */

    /* Sieve of Eratosthenes up to 'limit' */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime) return (seq_t)0;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Build prime list */
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++prime_cnt;
    idx_t *primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!primes) { free(is_prime); return (seq_t)0; }
    idx_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) primes[pc++] = i;
    free(is_prime);

    /* Accumulate result by prime exponents */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exp_in_product_range(0, n - 1, p)
                + exp_in_product_range(2 * n, 3 * n - 1, p)
                - exp_in_product_range(n, 2 * n - 1, p);
        if (e) result *= pow_seq((seq_t)p, e);
    }

    free(primes);
    return result;
}
