/*
 * A089190  a(n) = Floor[(Prime[n]+PrimePi[n])/2].
 * https://oeis.org/A089190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor[(Prime[n] + PrimePi[n]) / 2]
 * Range:     u8 n <= 54, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   realloc_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime[n]   : the n‑th prime (n ≥ 1).
   PrimePi[n] : π(n), the number of primes ≤ n.  */

/* The implementation builds a simple Eratosthenes sieve lazily.
   It keeps a global list of primes and extends it as needed.
   No floating arithmetic or external utilities are used.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/realloc_check.h"

/* ---------- global caching data ---------- */
static idx_t    sieve_limit      = 0;      /* largest number for which a sieve exists */
static uint8_t *is_prime_vec     = NULL;   /* bitmap of primality for 0 .. sieve_limit */
static uint32_t *primes_arr      = NULL;   /* array of discovered primes, sorted ascending */
static uint32_t primes_capacity  = 0;      /* allocated capacity for primes_arr */
static uint32_t primes_count     = 0;      /* current number of stored primes */

/* ---------- sieve construction ---------- */
static void build_sieve(idx_t limit) {
    if (limit < 2) limit = 2;                    /* at least 2 for a meaningful sieve */

    /* allocate (or reallocate) the primality vector */
    is_prime_vec = (uint8_t *)realloc_check(is_prime_vec,
                                            (size_t)(limit + 1));
    for (idx_t i = 0; i <= limit; ++i) is_prime_vec[i] = 1;   /* tentative primes */

    is_prime_vec[0] = 0;
    is_prime_vec[1] = 0;

    /* classic Eratosthenes loops */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime_vec[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_vec[j] = 0;

    /* rebuild the prime list */
    /* approximate capacity using limit / log(limit) + 10;  the exact value is \n
       irrelevant – we just want to avoid too many reallocations. */
    uint64_t cap = (limit > 1000) ? (limit / 10 + 100) : 100;
    if (cap > UINT32_MAX) cap = UINT32_MAX;

    primes_capacity = (uint32_t)cap;
    primes_arr = (uint32_t *)realloc_check(primes_arr,
                                           (size_t)(primes_capacity * sizeof(uint32_t)));

    primes_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime_vec[i]) {
            if (primes_count == primes_capacity) {
                primes_capacity *= 2;
                primes_arr = (uint32_t *)realloc_check(primes_arr,
                                                       (size_t)(primes_capacity *
                                                                sizeof(uint32_t)));
            }
            primes_arr[primes_count++] = (uint32_t)i;
        }
    }
    sieve_limit = limit;
}

/* ---------- extend sieve to cover at least 'limit' ---------- */
static void ensure_limit(idx_t limit) {
    if (limit <= sieve_limit) return;
    build_sieve(limit);
}

/* ---------- ensure we have at least 'n' primes (i.e. the n‑th prime is known) ---------- */
static void ensure_prime_n(idx_t n) {
    if (primes_count >= n) return;          /* already enough */

    /* start with a reasonable limit; if no sieve yet, begin with 2 */
    idx_t limit = (sieve_limit > 2) ? sieve_limit : 2;

    /* keep doubling the limit until we have enough primes */
    while (primes_count < n) {
        limit *= 2;
        if (limit == 0) limit = 2;           /* guard against overflow */
        ensure_limit(limit);
    }
}

/* ---------- π(n): number of primes ≤ n ---------- */
static seq_t prime_pi_of_n(idx_t n) {
    /* we assume primes_arr contains all primes up to at least 'n'        */
    /* binary search for first index with prime > n, the index itself is π(n) */
    idx_t lo = 0;
    idx_t hi = primes_count;
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        if ((idx_t)primes_arr[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;   /* lo is count of primes ≤ n */
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1; offset is 1 */

    /* make sure we know the n‑th prime */
    ensure_prime_n(n);

    /* ensure sieve covers at least n (needed for π(n)) */
    ensure_limit(n);

    /* nth prime (primes_arr is 0‑based) */
    uint32_t p_val = primes_arr[n - 1];

    /* π(n) */
    seq_t pi_val = prime_pi_of_n(n);

    /* compute floor[(p_val + pi_val) / 2] */
    seq_t p  = (seq_t)p_val;
    seq_t pi = pi_val;
    return (p + pi) / 2;
}
