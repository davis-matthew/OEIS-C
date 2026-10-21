/*
 * A098707  a(1) = 2; for n > 1, a(n) is the smallest number m greater than
 *          a(n-1) with a digit d > 1 occurring exactly once in decimal
 *          representations of m and a(n-1) and d = gcd(m, a(n-1)).
 * https://oeis.org/A098707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_once
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098707: a(1)=2; for n>1, a(n) is the smallest m > a(n-1) such that
   there exists a digit d>1 that occurs exactly once in the decimal
   representations of m and a(n-1) and d = gcd(m, a(n-1)).  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_once.h"

/* --------- helpers ------------------------------------------------- */

/* greatest common divisor for seq_t */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------- cache state -------------------------------------------- */

/* allocated size of cache, number of stored terms, cache pointer */
static size_t cache_alloc = 0;
static size_t cache_count = 0;
static seq_t *cache = NULL;

/* ensure the cache can store at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_alloc) return;
    size_t new_alloc = cache_alloc ? cache_alloc : 16;
    while (new_alloc < need) new_alloc <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    /* realloc may fail – in that case the existing cache remains usable */
    if (new_cache) {
        cache = new_cache;
        cache_alloc = new_alloc;
    }
}

/* --------- sequence function -------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    /* initialize cache with first term if empty */
    if (cache_count == 0) {
        ensure_cache(16);
        cache[0] = (seq_t)2;
        cache_count = 1;
    }

    /* extend cache up to required index */
    while (cache_count < (size_t)n) {
        seq_t prev = cache[cache_count - 1];
        seq_t m = prev + (seq_t)1;

        for (;;) {
            seq_t g = gcd_seq_t(m, prev);
            if (g > (seq_t)1 && g < (seq_t)10) {          /* digit d=0..9 only */
                int d = (int)g;
                if (digit_once(prev, d) && digit_once(m, d))
                    break;                               /* found next term */
            }
            m = m + (seq_t)1;
        }

        ensure_cache(cache_count + 1);
        cache[cache_count] = m;
        ++cache_count;
    }

    return cache[(size_t)n - 1];
}
