/*
 * A172121  Complement to A172120. Related to the ABC conjecture.
 * https://oeis.org/A172121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k (k ≥ 2) such that among all pairs (x,
 *            k‑x) with 1 ≤ x < k‑x and gcd(x,k)=1, the product of the radical
 *            values rad(x)·rad(k‑x) achieves its minimum exactly once. The
 *            function builds a radical sieve up to a dynamic limit and checks
 *            integers sequentially, caching found terms
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_uint64.h"
 
/* ---------- Small helpers ----------
 * Explicit integer GCD for 64‑bit values. */
/* ------------------------------------------------------------------------- */

 
/* ---------- Radical sieve and caching ----------
 * rad[i] = product of distinct prime divisors of i  (radical of i).
 * The sieve is rebuilt with a larger limit when needed. */
/* ------------------------------------------------------------------------- */
static seq_t   *rad      = NULL;    /* radical table (rad[0] unused) */
static idx_t    rad_limit = 0;      /* current maximum index of rad table */
 
static void rebuild_rad(idx_t limit)
{
    if (rad) free(rad);
    rad_limit = limit;
    rad = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    /* init to 1 */
    for (idx_t i = 0; i <= limit; ++i) rad[i] = (seq_t)1;
    /* sieve primes */
    char *is_comp = (char *)calloc(limit + 1, 1);
    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!is_comp[p])              /* p is prime */
        {
            for (idx_t m = p; m <= limit; m += p)
                rad[m] = rad[m] * (seq_t)p;
            for (idx_t m = p * p; m <= limit; m += p)
                is_comp[m] = 1;
        }
    }
    free(is_comp);
}
 
/* ---------- Check whether k belongs to the sequence ---------- */
/* ------------------------------------------------------------------------- */
static inline bool qualifies(idx_t k)
{
    if (k == 2) return true;   /* k=2 has no x but is included */
 
    size_t min_prod   = (size_t)-1;
    size_t count_min  = 0;
    idx_t  half = (k - 1) / 2;   /* x < k‑x ⇒ x ≤ half */
 
    for (idx_t x = 1; x <= half; ++x)
    {
        if (gcd_uint64((uint64_t)x, (uint64_t)k) != 1) continue;
        size_t prod = (size_t)rad[x] * (size_t)rad[k - x];
        if (prod < min_prod) {
            min_prod   = prod;
            count_min  = 1;
        } else if (prod == min_prod) {
            ++count_min;
        }
    }
    return count_min == 1;
}
 
/* ---------- Sequence generator with caching ---------- */
/* ------------------------------------------------------------------------- */
static seq_t *cache     = NULL;   /* stored terms of the sequence    */
static size_t cache_len = 0;      /* number of stored terms          */
static size_t cache_cap = 0;      /* allocated capacity              */
static idx_t  next_k    = 2;      /* next integer to test           */
 
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */
 
    /* initialise radical table once, with a generous limit */
    if (!rad) {
        rebuild_rad(200000);        /* covers n up to ~10⁴ comfortably */
    }
 
    /* generate terms until we have at least n of them  */
    while (cache_len < (size_t)n)
    {
        /* rebuild radical table if the candidate exceeds the limit */
        if (next_k > rad_limit) {
            rebuild_rad(next_k * 2 + 1);
        }
 
        if (qualifies(next_k)) {
            if (cache_cap <= cache_len) {   /* grow cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* realloc may fail; when it does, the old pointer is kept */
                if (!tmp) return (seq_t)0;  /* allocation failure → zero */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
