/*
 * A171932  Backwards van Eck transform of prime gaps, A001223.
 * https://oeis.org/A171932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Backwards van Eck transform of prime gaps (OEIS A171932)
   a(1) = 0
   For n > 1:
       let g(n) be the difference between the (n+1)-th and n-th primes
       (the n‑th prime gap, starting with g(1)=1, g(2)=2, …).
       a(n) = 0                               if g(n) has never appeared before
       a(n) = n - last_index_of_same_gap(g(n)) otherwise
   The implementation caches primes, last-gap occurrences, and a(n) values
   so that a(n) can be requested in any order.  All allocations are done
   with realloc and casted to satisfy both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -----  Static cache  ----- */
static seq_t *a_cache  = NULL;   /* cache of computed a(n)   */
static size_t cache_len = 0;     /* number of terms cached   */
static size_t cache_cap = 0;     /* allocated capacity       */

/* -----  Primes  ----- */
static uint64_t *prime_arr = NULL;
static size_t primes_len = 0;    /* number of primes stored  */
static size_t primes_cap = 0;    /* allocated capacity       */

/* -----  Last occurrence of each gap  ----- */
static idx_t *gap_last = NULL;   /* gap_last[g] = last 1‑based index where gap g appeared; 0 = never */
static size_t gap_cap = 0;       /* allocated capacity of gap_last array */

/* ------------------------------------------------------------------ */
/* Ensure that gap_last can hold index 'gap'; grow by powers of two.
   New entries are initialized to 0. */
static void ensure_gap_cap(uint64_t gap)
{
    if (gap < gap_cap) return;
    size_t new_cap = (gap_cap > 0) ? gap_cap : 1024;
    while (gap >= new_cap) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(gap_last, new_cap * sizeof(idx_t));
    if (!tmp) abort();           /* out of memory */
    /* initialize newly allocated part */
    memset(tmp + gap_cap, 0, (new_cap - gap_cap) * sizeof(idx_t));
    gap_last = tmp;
    gap_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure that prime_arr can hold at least (new_cap) primes. */
static void ensure_prime_cap(void)
{
    if (primes_len < primes_cap) return;
    size_t new_cap = (primes_cap > 0) ? 2 * primes_cap : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_arr = tmp;
    primes_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Simple deterministic primality test using previously found primes
   (sufficient for n up to about 10 000). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Ensure that prime_arr contains primes[0 … idx_needed] inclusive.
   That is, after the call primes_len > idx_needed. */
static void ensure_primes_upto_idx(idx_t idx_needed)
{
    if (primes_len > idx_needed) return;
    uint64_t candidate = (primes_len == 0) ? 2 : prime_arr[primes_len - 1] + 1;
    while (primes_len <= idx_needed) {
        while (!is_prime_uint64(candidate))
            ++candidate;
        ensure_prime_cap();
        prime_arr[primes_len++] = candidate;
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, but handled safely */

    /* Grow a_cache to at least n elements if necessary */
    if (cache_cap < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(a_cache, n * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        cache_cap = n;
    }

    /* Compute all terms up to n (in order 1 … n) if not yet cached */
    for (size_t t = cache_len; t < (size_t)n; ++t) {
        idx_t term_idx = (idx_t)(t + 1);          /* 1‑based index of a(t+1) */

        /* Ensure we have primes up to index term_idx (j = term_idx) */
        ensure_primes_upto_idx(term_idx);

        /* Compute the nth prime gap: p_{j+1} - p_j */
        uint64_t gap = prime_arr[term_idx] - prime_arr[term_idx - 1];

        /* Get last occurrence of this gap */
        ensure_gap_cap(gap);
        idx_t last = gap_last[gap];
        seq_t val;
        if (last == 0) {
            val = (seq_t)0;
        } else {
            val = (seq_t)(term_idx - last);
        }
        gap_last[gap] = term_idx;          /* update last occurrence */

        a_cache[t] = val;                  /* store the computed term */
    }

    cache_len = (size_t)n;                 /* now up to n cached */
    return a_cache[n - 1];
}
