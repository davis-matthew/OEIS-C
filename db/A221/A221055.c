/*
 * A221055  Primes of the form k*(k+1)*(k+2)/6+2 (i.e., two more than a
 *          tetrahedral number).
 * https://oeis.org/A221055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of the form k*(k+1)*(k+2)/6 + 2 (i.e. T_k + 2 where
 *            T_k is the k-th tetrahedral number)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 11, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
#include <vector>
#endif

/* static state */
static idx_t    next_k = 0;  /* smallest k not yet tested (k starts at 0) */

#ifdef __cplusplus
static std::vector<seq_t> prime_cache;
#else
static seq_t *prime_cache = NULL;
static size_t prime_len   = 0;   /* number of primes stored */
static size_t prime_cap   = 0;   /* capacity of the array */
#endif

/* Append a prime to the cache (grow array if needed) */
static void
append_prime(seq_t p)
{
#ifdef __cplusplus
    prime_cache.push_back(p);
#else
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap << 1;
        seq_t *tmp = realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        prime_cache = tmp;
        prime_cap = new_cap;
    }
    prime_cache[prime_len++] = p;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n < 1 */

    /* early return if required prime already cached */
#ifdef __cplusplus
    if (n <= prime_cache.size())
        return prime_cache[n - 1];
#else
    if (n <= prime_len)
        return prime_cache[n - 1];
#endif

    /* generate primes until the n-th one is found */
    for (;;) {
        seq_t term = binomial(next_k + 2, 3) + 2;   /* T_k + 2 */
        if (is_prime_big(term))
            append_prime(term);

        next_k++;   /* advance to next k regardless of primality */

#ifdef __cplusplus
        if (n <= prime_cache.size())
            return prime_cache[n - 1];
#else
        if (n <= prime_len)
            return prime_cache[n - 1];
#endif
    }
}
