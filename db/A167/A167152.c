/*
 * A167152  Lexicographically earliest injective sequence such that a(n) =
 *          A007953(a(a(n))), where A007953 = sum of digits (in base 10).
 * https://oeis.org/A167152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 41, u32 n <= 85, u64 n <= 172,
 *            u128 n <= 345, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167152 (lexicographically earliest injective sequence with a(n)=A007953(a(a(n)))).  
   Rule (conjectured in OEIS):
   For n ≥ 1:
     • if n < 10 or n already appears among a(1),…,a(n‑1), then
           a(n) = (n mod 9 + 1) * 10^⌊n/9⌋ – 1        (which equals n for n < 10)
     • otherwise, let k = n + 1.
           If k already appears among the preceding terms, use a(n) = k + 1;
           otherwise a(n) = k.  
   The sequence is injective and computed lazily; any order of calls is
   supported.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* static caching of already computed terms  */
static seq_t *cache      = NULL;   /* cache[0] stores a(1) */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure the cache can hold at least 'need' terms (0‑based).  */
static void ensure_cache_capacity(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* memory exhausted */
    cache = tmp;
    cache_cap = new_cap;
}

/* Linear search for a value among the first 'cnt' terms of the cache.  */
static bool contains(seq_t val, size_t cnt) {
    for (size_t i = 0; i < cnt; ++i)
        if (cache[i] == val) return true;
    return false;
}

/* Main function  */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for 0 or negative */

    /* initialisation on first call */
    if (cache_len == 0) {
        ensure_cache_capacity(16);
        cache[0] = (seq_t)1;          /* a(1) = 1 */
        cache_len = 1;
    }

    /* compute terms up to the requested index */
    while (cache_len < (size_t)n) {
        size_t idx = cache_len + 1;        /* next 1‑based index to compute */
        seq_t next_val;

        if (idx < 10) {
            /* For 1 ≤ n ≤ 9 the term equals the index itself */
            next_val = (seq_t)idx;
        } else {
            /* check whether idx already appears in the sequence */
            bool in_seq = contains((seq_t)idx, cache_len);

            if (in_seq) {
                /* a(idx) = (idx mod 9 + 1) * 10^⌊idx/9⌋ – 1  */
                idx_t mod9   = idx % 9;
                idx_t power  = idx / 9;            /* floor division  */
                seq_t pow10v = pow10(power);       /* 10^power in seq_t  */
                seq_t factor = (seq_t)(mod9 + 1);
                next_val = pow10v * factor - (seq_t)1;
            } else {
                /* candidate = idx + 1, unless already seen */
                seq_t cand = (seq_t)(idx + 1);
                if (contains(cand, cache_len)) {
                    /* if cand is already used, skip to the next unused integer */
                    do { cand++; } while (contains(cand, cache_len));
                }
                next_val = cand;
            }
        }

        /* store computed term */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len] = next_val;
        cache_len++;
    }

    /* return the requested term */
    return cache[n - 1];
}
