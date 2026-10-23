/*
 * A164969  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164969: number of reduced words of length n in the
   Coxeter group with 27 generators S_i and relations
   (S_i)^2 = (S_i S_j)^8 = I.
   Recurrence (derived from the generating function)
     a(0) = 1
     a(1) = 27
     a(2) = 702
     a(3) = 18252
     a(4) = 474552
     a(5) = 12338352
     a(6) = 320797152
     a(7) = 8340725952
     a(8) = 216858874401
     for n ≥ 9:
        a(n) = 25*(a(n-1)+a(n-2)+…+a(n-7)) - 325*a(n-8)
   The only exceptional term in the recurrence is a(8), where an
   additive constant 1 compensates for the numerator of the
   generating function.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* The types seq_t (value type) and idx_t (index type) are supplied
   by the build environment.  They are not defined here. */

/* Initial values 0..7 (indices 0–7) */
static const seq_t init_vals[8] = {
    (seq_t)1,            /* a(0) */
    (seq_t)27,           /* a(1) */
    (seq_t)702,          /* a(2) */
    (seq_t)18252,        /* a(3) */
    (seq_t)474552,       /* a(4) */
    (seq_t)12338352,     /* a(5) */
    (seq_t)320797152,    /* a(6) */
    (seq_t)8340725952    /* a(7) */
};

/* Cache for already computed terms.  cache[0] .. cache[cache_limit-1]
   are valid.  cache_limit is the next free index. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_limit = 0;

/* Ensure the cache can hold at least need elements. */
static void
ensure_cache(idx_t need)
{
    if (cache_cap >= (size_t)need)
        return;
    size_t new_cap = need * 2;
    if (new_cap < 16)
        new_cap = 16;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();  /* memory exhaustion is fatal */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) for any n ≥ 0.  The function may be called in any
   order; already computed values are cached. */
seq_t a(idx_t n)
{
    /* Ensure the cache holds values up to n. */
    if (n >= cache_limit) {
        /* If this is the very first call, initialize first 8 entries. */
        if (cache_limit == 0) {
            ensure_cache(8);
            for (idx_t i = 0; i < 8; ++i)
                cache[i] = init_vals[i];
            cache_limit = 8;
        }
        /* Compute missing terms up to n. */
        for (idx_t i = cache_limit; i <= n; ++i) {
            seq_t val;
            if (i < 8) {
                /* Should already be present; safety check. */
                val = init_vals[i];
            } else {
                /* Sum of the previous 7 terms. */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 7; ++k)
                    sum += cache[i - k];
                val = 25 * sum;                /* 25 * sum of a(n-1) .. a(n-7) */
                val -= 325 * cache[i - 8];      /* – 325·a(n-8) */
                if (i == 8)                     /* add the numerator contribution */
                    val += (seq_t)1;
            }
            cache[i] = val;
        }
        cache_limit = n + 1;
    }
    return cache[n];
}
