/*
 * A173287  Intersection of A173286 and A173269.
 * https://oeis.org/A173287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = intersection of A173286 and A173269: integers m for which
 *            2*p(p(p(m)))-3, 3*p(p(p(m)))-2, 2*p(p(m))-3 and 3*p(p(m))-2 are
 *            all prime. Offset: 1 (a(1) = 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1;   /* next candidate to test */

/* Test whether m satisfies the four primality conditions. */
static bool good_candidate(idx_t m)
{
    idx_t p1 = get_prime(m);          /* p(m)  */
    idx_t p2 = get_prime(p1);         /* p(p(m)) */
    idx_t p3 = get_prime(p2);         /* p(p(p(m))) */

    idx_t v1 = (idx_t)((unsigned __int128)2 * p3 - 3);   /* 2*p3-3   */
    idx_t v2 = (idx_t)((unsigned __int128)3 * p3 - 2);   /* 3*p3-2   */
    if (!is_prime(v1) || !is_prime(v2))
        return false;

    idx_t v3 = (idx_t)((unsigned __int128)2 * p2 - 3);   /* 2*p2-3   */
    idx_t v4 = (idx_t)((unsigned __int128)3 * p2 - 2);   /* 3*p2-2   */
    if (!is_prime(v3) || !is_prime(v4))
        return false;

    return true;
}

/* Grow the cache so that it contains at least n terms. */
static void ensure_n(idx_t n)
{
    while (cache_len < n) {
        while (true) {
            idx_t m = last_checked++;
            if (good_candidate(m)) {
                if (cache_len == cache_cap)
                    grow_seq((seq_t **)&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)m;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not defined for n=0; return 0 for safety */
    ensure_n(n);
    return cache[n - 1];
}
