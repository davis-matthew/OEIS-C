/*
 * A059790  Distance between 2*(n-th prime) and nearest prime.
 * https://oeis.org/A059790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distance between 2*(n-th prime) and nearest prime.
   a(n) = min(|2·p_n − q|) over all primes q ≠ 2·p_n.
   p_n = the n-th prime (p_1 = 2).  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions supplied by the build. */
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* --- Cache of the first k primes ------------------------------------ */
static idx_t *prime_cache = NULL;   /* 0‑based array of primes */
static size_t   prime_cap = 0;      /* allocated capacity */
static size_t   prime_cnt = 0;      /* number of primes stored */

/* Ensure that prime_cache contains at least need primes. */
static void
ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* grow capacity */
    if (prime_cap == 0)
        prime_cap = 16;
    while (prime_cap < need)
        prime_cap <<= 1;

    prime_cache = (idx_t *)realloc(prime_cache,
                                   prime_cap * sizeof(idx_t));
    if (!prime_cache)
        return;                    /* allocation failure – let future
                                      calls crash (not expected). */

    /* generate missing primes */
    if (prime_cnt == 0) {
        prime_cache[prime_cnt++] = (idx_t)2;
    }
    while (prime_cnt < need) {
        uint64_t nxt = next_prime((uint64_t)prime_cache[prime_cnt - 1]);
        prime_cache[prime_cnt++] = (idx_t)nxt;
    }
}

/* --- Main sequence function ------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for 0, safe return */

    /* get the n-th prime */
    ensure_primes((size_t)n);
    idx_t p_n = prime_cache[n - 1];   /* 0‑based indexing */

    /* even number 2·p_n (always > 2, not prime) */
    idx_t M = p_n << 1;               /* p_n * 2 using bit shift */

    /* nearest prime below M */
    idx_t lower = M - 1;
    while (lower >= 2 && !is_prime(lower))
        --lower;
    idx_t dist_lower = M - lower;

    /* nearest prime above M */
    idx_t upper = M + 1;
    while (!is_prime(upper))
        ++upper;
    idx_t dist_upper = upper - M;

    /* return the smaller distance, cast to seq_t */
    return (dist_lower < dist_upper ? (seq_t)dist_lower : (seq_t)dist_upper);
}
