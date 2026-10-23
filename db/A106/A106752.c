/*
 * A106752  Numbers of prime factors of k, k defined in A117078.
 * https://oeis.org/A106752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of k, where k is the smallest
 *            integer such that prime(n+1) = prime(n) + (prime(n) mod k), or 0
 *            when no such k exists
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega_uint64, estimate_prime_upper_bound, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"
#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/bigomega_uint64.h"

static uint32_t *prime_list = NULL;   /* array of generated primes (uint32_t) */
static size_t prime_count = 0;       /* number of primes stored */

/* Ensure that primes up to index 'need' (0‑based) are available. */
static void ensure_primes(idx_t need)
{
    if (prime_list && prime_count > (size_t)need)
        return;                                    /* already enough */

    /* release old storage */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* estimate enough primes for the requested index */
    uint64_t limit = estimate_prime_upper_bound((idx_t)need + 10) + 100;
    if (limit > 0xffffffffu) limit = 0xffffffffu;   /* keep within uint32 */

    uint32_t *tmp = NULL;
    size_t tmp_cnt = 0;
    simple_sieve((uint32_t)limit, &tmp, &tmp_cnt);

    prime_list = tmp;
    prime_count = tmp_cnt;
    /* The sieve may produce fewer primes than requested; in that unlikely
       case we would try again, but for n ≤ 10000 the estimate is sufficient. */
}

seq_t a(idx_t n)
{
    if (n == 0)        /* the offset is 1; n>=1 is required */
        return 0;

    /* obtain the first n+1 primes needed: prime(n) and prime(n+1) */
    ensure_primes((idx_t)n);   /* need indices up to n (0‑based) */
    if (prime_count <= (size_t)n)   /* safety check, should not happen */
        return 0;

    uint64_t p = (uint64_t)prime_list[n - 1];   /* prime(n)   */
    uint64_t q = (uint64_t)prime_list[n];       /* prime(n+1) */

    uint64_t d = q - p;                         /* q > p, so d > 0 */

    int64_t M = (int64_t)2 * p - (int64_t)q;     /* M = 2*p - q = p - d */
    if (M <= 0)
        return 0; /* no divisor satisfies k > d */

    uint64_t m = (uint64_t)M;
    uint64_t best_k = 0;

    /* find smallest divisor of m that is > d */
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            if (i > d && (best_k == 0 || i < best_k))
                best_k = i;
            uint64_t j = m / i;
            if (j > d && (best_k == 0 || j < best_k))
                best_k = j;
        }
    }

    if (best_k == 0)
        return 0;    /* no suitable k found */

    /* count number of prime factors (with multiplicity) of k */
    return bigomega_uint64(best_k);
}
