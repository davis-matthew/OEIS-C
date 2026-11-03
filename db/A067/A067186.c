/*
 * A067186  Numbers n such that C(n) = (n^2 + n + 2)/2 is prime.
 * https://oeis.org/A067186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 935 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n whose cake integer  
   C(n) = (n^2 + n + 2) / 2 is prime.  
   The function returns the n‑th such number, 1‑based. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Return the n‑th element of OEIS A067186.  */
seq_t a(idx_t n)
{
    /* Off‑by‑one offset: a(1) is the first term. */
    if (n == 0)      /* undefined for offset 1, simply return 0 */
        return 0;

    /* Cached results. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* allocated capacity */
    static size_t term_count  = 0;   /* number of terms stored */
    static idx_t  last_checked = 0;  /* largest index examined */

    /* Return cached value if already known. */
    if (n <= term_count)
        return cache[n - 1];

    /* Search forward until we have at least n terms. */
    idx_t cand = last_checked + 1;
    while (term_count < n) {
        /* C(cand) = (cand^2 + cand + 2) / 2 = cand*(cand+1)/2 + 1 */
        seq_t cn = ((seq_t)cand * (cand + 1)) / 2 + 1;

        if (isPrimeGeneric(cn)) {
            /* Grow the cache if needed. */
            if (term_count == cache_size) {
                size_t new_size = cache_size ? cache_size * 2 : 16;
                seq_t *tmp_ptr = seq_alloc_new_array(new_size);
                if (!tmp_ptr)
                    abort();                 /* allocation failure */
                /* copy old data */
                for (size_t i = 0; i < term_count; ++i)
                    tmp_ptr[i] = cache[i];
                if (cache)
                    seq_free_array(cache);
                cache = tmp_ptr;
                cache_size = new_size;
            }
            cache[term_count++] = (seq_t)cand;   /* store the index itself */
        }
        cand++;
    }

    last_checked = cand - 1;
    return cache[n - 1];
}
