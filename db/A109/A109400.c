/*
 * A109400  For all k: the first k numbers followed by the first k primes.
 * https://oeis.org/A109400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n lies in block k (k(k-1)+1 … k(k+1)): if n ≤ k² → return
 *            n – k(k-1) (the natural numbers 1 … k) else → return prime( n –
 *            k² ) where k is the unique integer with k(k-1) < n ≤ k(k+1)
 * Range:     u8 n <= 3079, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Prime cache.  prime_list[i] holds the (i+1)-th prime.  */
static uint64_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;

/* Return floor(sqrt(x)) for a 64‑bit unsigned integer. */
static uint64_t uint64_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;       /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Verify whether the candidate is prime using the existing cache. */
static int is_prime_candidate(uint64_t cand)
{
    if (cand < 2) return 0;
    uint64_t limit = uint64_isqrt(cand);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit)
            break;
        if (cand % p == 0)
            return 0;
    }
    return 1;
}

/* Grow the cache to contain at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Grow the array if necessary. */
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 4;
        while (new_cap < need)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp)
            return;          /* Allocation failed – leave the cache incomplete. */
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Start searching for primes after the last known one. */
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;

    while (prime_cnt < need) {
        if (is_prime_candidate(candidate))
            prime_list[prime_cnt++] = candidate;
        ++candidate;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined for the offset 1. */
        return (seq_t)0;

    /* Find the block index k such that k(k-1) < n ≤ k(k+1). */
    uint64_t k = 1;
    while ((unsigned __int128)k * (k + 1) < n)
        ++k;

    /* Offset inside the block: 1 … 2k. */
    uint64_t start = (k - 1) * k + 1;      /* first index of block k */
    uint64_t m     = n - start + 1;        /* 1‑based position within the block */

    if (m <= k)                       /* natural part of the block */
        return (seq_t)m;

    /* prime part of the block */
    uint64_t prime_index = m - k;      /* 1‑based index of the prime */
    ensure_primes(prime_index);        /* guarantee the cache contains it */
    return (seq_t)(prime_list[prime_index - 1]);
}
