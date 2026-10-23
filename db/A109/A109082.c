/*
 * A109082  Depth of rooted tree having Matula-Goebel number n.
 * https://oeis.org/A109082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; if n is the t‑th prime, a(n)=1+a(t); if n=t*s composite,
 *            a(n)=max(a(t),a(s)). Computed by memoised recursion, with
 *            on‑demand prime table
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Global state for memoisation and prime handling */
static seq_t *depth_cache = NULL;
static bool *depth_known = NULL;
static idx_t cache_cap = 0;

static idx_t *prime_list = NULL;
static idx_t  prime_count = 0;
static idx_t  prime_cap   = 0;

/* Ensure that prime_list contains all primes up to 'limit'. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_cap) return;
    /* Allocate sieve */
    bool *is_comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp) exit(1);
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            ++cnt;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    /* Store primes */
    idx_t *newlist = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!newlist) exit(1);
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            newlist[pos++] = i;
    free(is_comp);
    free(prime_list);
    prime_list = newlist;
    prime_count = cnt;
    prime_cap = limit;
}

/* Return 1‑based index of prime n, or 0 if n is not prime. */
static idx_t prime_index(idx_t n)
{
    if (n < 2) return 0;
    if (n > prime_cap) ensure_primes_upto(n);
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < prime_count && prime_list[lo] == n)
        return (idx_t)(lo + 1);
    return 0;
}

/* Ensure cache large enough to hold values up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(depth_cache, new_cap * sizeof(seq_t));
    bool *new_known = (bool *)realloc(depth_known, new_cap * sizeof(bool));
    if (!new_cache || !new_known) exit(1);
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_known[i] = false;
    depth_cache = new_cache;
    depth_known = new_known;
    cache_cap = (idx_t)new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;         /* undefined indices handled gracefully */
    ensure_cache(n);
    if (depth_known[n]) return depth_cache[n];

    seq_t result;
    if (n == 1) {
        result = 0;
    } else {
        /* Check for prime */
        idx_t t = prime_index(n);
        if (t != 0) {            /* n is prime */
            result = (seq_t)1 + a(t);
        } else {                  /* composite */
            seq_t max_depth = 0;
            /* iterate over divisors up to sqrt(n) */
            for (idx_t d = 2; d * d <= n; ++d) {
                if (n % d == 0) {
                    seq_t d_depth = a(d);
                    seq_t q_depth = a(n / d);
                    seq_t cur = d_depth > q_depth ? d_depth : q_depth;
                    if (cur > max_depth) max_depth = cur;
                }
            }
            result = max_depth;
        }
    }
    depth_cache[n] = result;
    depth_known[n] = true;
    return result;
}
