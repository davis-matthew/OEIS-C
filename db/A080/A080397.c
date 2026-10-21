/*
 * A080397  Largest squarefree number dividing central binomial coefficient
 *          A000984(n).
 * https://oeis.org/A080397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all distinct primes that divide the central
 *            binomial coefficient C(2*n,n) (the radical of C(2*n,n))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t limit = n << 1;           /* 2*n */
    size_t sz = (size_t)(limit + 1);

    /* Sieve of Eratosthenes up to 2*n */
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return (seq_t)0;           /* allocation failure */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    if (limit >= 0)  is_prime[0] = false;
    if (limit >= 1)  is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* collect primes */
    size_t primes_cap = limit / 2 + 1;
    int *primes = (int *)malloc(primes_cap * sizeof(int));
    if (!primes) { free(is_prime); return (seq_t)0; }
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {          /* grow if needed */
                primes_cap *= 2;
                int *tmp = (int *)realloc(primes, primes_cap * sizeof(int));
                if (!tmp) { free(primes); free(is_prime); return (seq_t)0; }
                primes = tmp;
            }
            primes[primes_cnt++] = (int)i;
        }
    }
    free(is_prime);

    seq_t rad = (seq_t)1;

    /* For each prime, check if it divides C(2n,n) */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        idx_t p = (idx_t)primes[idx];
        idx_t vp = 0;
        idx_t power = p;
        while (power <= limit) {
            vp += (limit / power) - 2 * (n / power);
            if (power > (n >> 1)) { /* avoid overflow when multiplying */
                if (power > (idx_t)((~(idx_t)0) / p))
                    break;
            } else {
                if (power > (idx_t)((~(idx_t)0) / p))
                    break;
            }
            power *= p;
        }
        if (vp > 0)
            rad *= (seq_t)p;
    }

    free(primes);
    return rad;
}
