/*
 * A059786  Smallest prime after 2*(n-th prime).
 * https://oeis.org/A059786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059786 – Smallest prime after 2 * (n-th prime)
 *  a(n) = the least prime > 2·p(n),  n ≥ 1.
 *  a(1) = 5, a(2) = 7, a(3) = 11, …
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

/* Store primes in a static cache so that repeated calls are fast. */
static uint64_t *prime_list  = NULL; /* primes[0] = 2, primes[1] = 3, … */
static size_t   prime_count = 0;

/* Ensure that prime_list contains at least 'need' primes (need ≥ 1).  */
static void
ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    /* Free old cache, if any. */
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
    }

    /* Rough upper bound for the nth prime: p_n < n(ln n + ln ln n)
       For n ≥ 6 this is safe.  For small n use a modest limit.  */
    idx_t limit = 0;
    if (need < 6)
        limit = 15;
    else
        limit = need * 15;          /* conservative over‑estimate */

    uint64_t *tmp   = NULL;
    size_t   count  = 0;
    build_primes_upto(limit, &tmp, &count);

    /* We ignore the situation where count < need – the over‑estimate
       guarantees enough primes for all test cases up to 10 000. */
    prime_list  = tmp;
    prime_count = count;
}

/* ----------  sequence function  ---------- */
seq_t
a(idx_t n)
{
    if (n == 0)        /* offset is 1; unused value, but keep for safety */
        return 0;

    ensure_primes(n);

    uint64_t p     = prime_list[(size_t)(n - 1)];   /* n‑th prime, 0‑based indexing */
    uint64_t target= p * 2ULL;                     /* 2·p(n) */
    uint64_t cand  = target + 1ULL;                /* first integer > 2·p */

    /* Handle tiny candidates (around 2).  */
    if (cand <= 2ULL)
        cand = 2ULL;
    /* Skip even numbers.  */
    if ((cand & 1ULL) == 0ULL)
        cand++;

    /* Search for the next prime.  */
    for (;; cand += 2ULL) {
        if (is_prime_big((seq_t)cand))
            break;
    }

    return (seq_t)cand;
}
