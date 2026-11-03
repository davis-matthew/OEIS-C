/*
 * A217916  a(n) = prime(n) - Sum_{k=1..A002024(n)-1} a(n-k), where A002024(m)
 *          = [sqrt(2*m)+1/2] is "n appears n times".
 * https://oeis.org/A217916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - Σ_{k=1..A002024(n)-1} a(n-k), a(1)=2 where
 *            A002024(m) = smallest k with k(k+1)/2 ≥ m (the “n appears n
 *            times” sequence)
 * Range:     u8 n <= 663, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 87 terms through n = 9957 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* smallest k such that k(k+1)/2 >= m  (m >= 1) */
static idx_t a002024(idx_t m)
{
    idx_t low = 1, high = m;          /* upper bound is m */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if ((mid * (mid + 1)) / 2 >= m)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static seq_t *cache = NULL;          /* cache[0] holds a(1) */
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ensure that cache contains a(1)..a(n) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* grow capacity if needed */
    size_t need = n;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
        if (!new_buf)
            exit(1);                    /* allocation failed */
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
            continue;
        }

        uint32_t p = get_prime(i);           /* prime(i) fits in uint32_t */
        idx_t k = a002024(i);                /* number of terms to include */

        seq_t sum = 0;
        for (idx_t j = 1; j < k; ++j) {
            sum += cache[i - j - 1];
        }

        cache[i - 1] = (seq_t)p - sum;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;                     /* undefined for n < 1 */
    }
    ensure_cache(n);
    return cache[n - 1];                     /* a(1) is at cache[0] */
}
