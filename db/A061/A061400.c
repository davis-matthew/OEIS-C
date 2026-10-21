/*
 * A061400  Primes p such that there is no squarefree number between p and the
 *          next prime.
 * https://oeis.org/A061400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 652, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A061400
 *  a(n) = the n-th prime p such that every integer m with p < m < next_prime(p)
 *  is composite and not squarefree.
 *  (Equivalently, no squarefree number lies between p and its successor prime.)
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper prototypes from the utils library */
#include "../utils/is_squarefree.h"
#include "../utils/next_prime.h"

/* Cache for already‑computed terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_chkr = 1;   /* last prime that was examined */

/* Ensure the cache can hold at least “needed” more entries. */
static void grow_cache(size_t needed)
{
    if (cache_len + needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < cache_len + needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Build the cache until it contains at least “target” terms. */
static void build_until(idx_t target)
{
    if (cache_len >= (size_t)target)
        return;

    uint64_t p = last_chkr;          /* last prime that was checked */
    while (cache_len < (size_t)target) {
        /* Find the next prime. */
        p = next_prime(p);
        uint64_t q = next_prime(p);  /* prime immediately after p */

        /* Check that all numbers between p and q are non-squarefree. */
        bool ok = true;
        for (uint64_t m = p + 1; m < q; ++m) {
            if (is_squarefree((seq_t)m)) {
                ok = false;
                break;
            }
        }

        if (ok) {                    /* p satisfies the condition */
            grow_cache(1);
            cache[cache_len++] = (seq_t)p;
        }

        last_chkr = p;                /* move to the next prime for the loop */
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n = 0; return 0 to avoid crash */
        return 0;
    build_until(n);
    return cache[n - 1];
}
