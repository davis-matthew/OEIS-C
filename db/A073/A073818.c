/*
 * A073818  a(n) = max(prime(i)*(n+1-i) | 1 <= i <= n).
 * https://oeis.org/A073818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max{ prime(i)*(n+1-i) : 1 <= i <= n }
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime(i) is the i-th prime (1‑based).  The implementation keeps a cached
 * list of primes that is extended on demand.  For each n it computes the
 * maximum product using the cached primes.  The routine works for any
 * n >= 1 and is independent of call order. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of primes up to the required nth prime.
 * prime_vals[0] = 2, prime_vals[1] = 3, ...                  */
static uint64_t *prime_vals = NULL;
static uint64_t prime_cnt   = 0;

/* Ensure that prime_vals contains at least n primes. */
static void
ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;                     /* already have enough */

    /* grow cache buffer if necessary */
    if (prime_vals == NULL) {
        prime_cnt   = 0;
        prime_vals  = NULL;
    }

    /* generate primes until we have n primes */
    uint64_t candidate = (prime_cnt == 0) ? 2ULL : prime_vals[prime_cnt - 1] + 1ULL;

    while (prime_cnt < n) {
        /* Assume prime until a divisor is found */
        bool is_prime = true;
        for (uint64_t i = 0; i < prime_cnt && prime_vals[i] * prime_vals[i] <= candidate; ++i) {
            if (candidate % prime_vals[i] == 0ULL) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* reallocate larger buffer if needed */
            size_t new_cnt = prime_cnt + 1;
            uint64_t *tmp  = (uint64_t *)realloc(prime_vals, new_cnt * sizeof(uint64_t));
            if (tmp == NULL) {
                /* allocation failure – terminate */
                exit(1);
            }
            prime_vals = tmp;
            prime_vals[prime_cnt] = candidate;
            ++prime_cnt;
        }
        ++candidate;
    }
}

/* -------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* out of offset – unspecified */

    ensure_primes(n);

    seq_t max_product = 0;
    /* i runs from 1 to n inclusive.  prime_vals is 0‑indexed. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prime  = (seq_t)prime_vals[i - 1];
        seq_t mult   = (seq_t)(n + 1 - i);
        seq_t product = prime * mult;
        if (product > max_product)
            max_product = product;
    }
    return max_product;
}
