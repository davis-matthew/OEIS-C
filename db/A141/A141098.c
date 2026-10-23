/*
 * A141098  Even numbers not representable as the sum of two coprime composite
 *          numbers.
 * https://oeis.org/A141098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th even number not equal to the sum of two coprime
 *            composite numbers. The sequence stops at 210 (last term)
 * Range:     u8 n <= 43, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

/* Static data structures */
static bool *comp = NULL;            /* composite marker array */
static uint64_t *odd_comps = NULL;    /* list of odd composite numbers */
static size_t odd_len = 0;            /* number of odd composites stored */

static seq_t *cache = NULL;          /* cache of sequence values */
static size_t cache_len = 0;          /* number of cached terms */
static size_t cache_cap = 0;          /* cache capacity */

/* Helper to grow the cache array */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) exit(1);
    cache = newp;
    cache_cap = new_cap;
}

/* Precompute composite markers and odd composites up to 'limit' */
static void precompute_composites(uint64_t limit)
{
    comp = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!comp) exit(1);
    for (uint64_t i = 0; i <= limit; ++i) comp[i] = false;

    uint64_t sq = (uint64_t)sqrt((long double)limit);
    for (uint64_t i = 2; i <= sq; ++i)
    {
        if (!comp[i]) {               /* i is prime */
            for (uint64_t j = i * i; j <= limit; j += i) comp[j] = true;
        }
    }

    /* Build list of odd composite numbers */
    size_t cap = 64;
    odd_comps = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!odd_comps) exit(1);
    odd_len = 0;
    for (uint64_t i = 9; i <= limit; i += 2)
    {
        if (comp[i]) {
            if (odd_len >= cap) {
                cap *= 2;
                odd_comps = (uint64_t*)realloc(odd_comps, cap * sizeof(uint64_t));
                if (!odd_comps) exit(1);
            }
            odd_comps[odd_len++] = i;
        }
    }
}

/* Test if an even number can be written as sum of two coprime odd composites */
static bool has_representation(uint64_t even_val)
{
    if (even_val < 18) return false;   /* smallest possible sum is 9+9 */

    for (size_t i = 0; i < odd_len; ++i)
    {
        uint64_t a = odd_comps[i];
        if (a > even_val / 2) break;
        uint64_t b = even_val - a;
        if (b < 9) continue;
        if (!comp[b]) continue;          /* b must be composite */
        if ((b & 1) == 0) continue;      /* b must be odd */
        if (gcd((seq_t)a, (seq_t)b) == 1) 
            return true;
    }
    return false;
}

/* Build the sequence up to the last term (210) */
static void build_sequence(void)
{
    uint64_t limit = 210;
    uint64_t current_even = 2;
    while (current_even <= limit)
    {
        if (!has_representation(current_even))
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = current_even;
        }
        current_even += 2;
    }
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but avoid crash */

    static int initialized = 0;
    if (!initialized)
    {
        precompute_composites(210);
        cache_cap = 0; cache = NULL; cache_len = 0;
        ensure_cache_cap(16);
        build_sequence();
        initialized = 1;
    }

    if (n > cache_len) return 0;  /* beyond defined terms */
    return cache[n - 1];
}
