/*
 * A038906  Primes p such that 31 is not a square mod p.
 * https://oeis.org/A038906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that (31|p) = -1 (Legendre symbol)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 813 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* static cache of sequence terms */
static seq_t *cache = NULL;   /* array of qualifying primes */
static idx_t cache_cap = 0;   /* capacity of the cache            */
static idx_t cache_len = 0;   /* number of primes stored           */
static idx_t last_checked = 2;/* last integer examined for prime   */

/* enlarge the cache until it can hold at least 'need' elements */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? (cache_cap << 1) : 16ULL;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    if (!new_cache) abort();          /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* a(n) – n-th term of A038906 */
seq_t a(idx_t n)
{
    if (n == 0) abort();                 /* offset is 1 */

    /* ensure cache contains at least n terms */
    while (cache_len < n) {
        idx_t cand = last_checked + 1;
        if (cand <= 2) cand = 3;
        else if ((cand & 1ULL) == 0) cand++;   /* start at odd number */

        for (;;) {
            if (is_prime(cand)) {
                /* Legendre symbol (31|cand); jacobi_symbol returns -1, 0 or 1 */
                int ls = jacobi_symbol(31ULL, cand);
                if (ls == -1) {
                    /* qualifies – store it in the cache */
                    grow_cache(cache_len + 1);
                    cache[cache_len] = (seq_t)cand;
                    cache_len++;
                    last_checked = cand;
                    break;
                }
            }
            cand += 2;                       /* next odd candidate */
            if (cand <= last_checked) {      /* overflow guard */
                abort();                      /* cannot reach the requested term */
            }
        }
    }

    return cache[n - 1];
}
