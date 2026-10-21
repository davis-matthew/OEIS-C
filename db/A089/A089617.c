/*
 * A089617  Number of primes between n^2 and (n+1/7)^2.
 * https://oeis.org/A089617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = pi(floor((n + 1/7)^2)) – pi(n^2) where pi(x) is the
 *            number of primes ≤ x. The upper bound is floor((7n+1)^2/49)
 *            which equals floor((n + 1/7)^2). The routine keeps a global
 *            sieve and prime list for all calls; it rebuilds the sieve only
 *            when a larger value of n is requested. All arithmetic u...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   upper_bound_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/upper_bound_for_n.h"

/* Global data retained between calls. */
static uint8_t  *sieve_flag   = NULL;   /* 1 = prime, 0 = composite */
static uint32_t *prime_list   = NULL;   /* list of primes */
static size_t    prime_count  = 0;      /* number of primes stored   */
static uint64_t  current_limit = 0;     /* current sieve upper bound */
static bool      sieve_ready   = false;

/* Build a sieve up to 'limit' (inclusive) and fill prime_list. */
static void build_sieve(uint64_t limit)
{
    size_t sz = (size_t)(limit + 1);
    /* free old allocation if any */
    free(sieve_flag);
    free(prime_list);

    sieve_flag = (uint8_t*)malloc(sz);
    if (!sieve_flag) exit(EXIT_FAILURE);
    memset(sieve_flag, 1, sz);          /* assume all prime initially   */
    sieve_flag[0] = 0;
    if (sz > 1) sieve_flag[1] = 0;

    /* simple O(N log log N) sieve */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (sieve_flag[p]) {
            for (uint64_t j = p * p; j <= limit; j += p) {
                sieve_flag[j] = 0;
            }
        }
    }

    /* count primes to allocate prime_list */
    size_t need = 0;
    for (size_t i = 2; i < sz; ++i) if (sieve_flag[i]) ++need;
    prime_list = (uint32_t*)malloc(need * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i) {
        if (sieve_flag[i]) prime_list[idx++] = (uint32_t)i;
    }
    prime_count = need;

    current_limit = limit;
    sieve_ready = true;
}

/* Ensure sieve contains all primes up to the required upper bound for n. */
static void ensure_sieve(uint64_t n)
{
    if (sieve_ready && current_limit >= upper_bound_for_n(n)) {
        return;   /* already sufficient */
    }
    /* enlarge sieve */
    build_sieve(upper_bound_for_n(n));
}

/* Binary search: count primes ≤ x in prime_list. */
static uint64_t count_leq(uint32_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;   /* number of primes ≤ x */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset starts at 1 */
    uint64_t u = (uint64_t)n;
    ensure_sieve(u);

    uint64_t upper = upper_bound_for_n(u);
    uint64_t lower = u * u;   /* n² */

    uint64_t cnt_upper = count_leq((uint32_t)upper);
    uint64_t cnt_lower = count_leq((uint32_t)lower);

    return (seq_t)(cnt_upper - cnt_lower);
}
