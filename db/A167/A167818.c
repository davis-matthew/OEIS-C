/*
 * A167818  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 35*(a(n-1)+a(n-2)+...+a(n-14)) - 630*a(n-15)  for n > 15
   a(0)=1, a(1)=37, a(2)=1332, a(3)=47952, a(4)=1726272,
   a(5)=62145792, a(6)=2237248512, a(7)=80540946432,
   a(8)=2899474071552, a(9)=104381066575872,
   a(10)=3757718396731392, a(11)=135277862282330112,
   a(12)=4870003042163884032, a(13)=175320109517899825152,
   a(14)=6311523942644393705472   */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of elements stored (>=1) */

/* initialize cache with the first 15 terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* string representations of the first 15 terms */
    const char *terms[15] = {
        "1",
        "37",
        "1332",
        "47952",
        "1726272",
        "62145792",
        "2237248512",
        "80540946432",
        "2899474071552",
        "104381066575872",
        "3757718396731392",
        "135277862282330112",
        "4870003042163884032",
        "175320109517899825152",
        "6311523942644393705472"
    };
    cache_cap = 15;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < 15; ++i)
        cache[i] = seq_from_str(terms[i]);
}

/* grow the cache up to at least index n (inclusive) */
static void ensure_cache(idx_t n)
{
    init_cache();
    /* no growth needed */
    if ((idx_t)cache_cap > n)
        return;

    /* allocate a little more space first */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap < cache_cap + 1)
        new_cap = cache_cap + 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)  /* allocation failed – reasonable to abort silently */
        return;
    cache = tmp;

    /* fill missing entries */
    for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
        if (i == 15) {
            /* special case: RHS of generating function contributes +1 */
            seq_t s = 0;
            for (int j = 1; j <= 14; ++j)
                s += (seq_t)35 * cache[(size_t)(i - j)];
            s -= (seq_t)630 * cache[(size_t)(i - 15)];
            s += (seq_t)1;
            cache[(size_t)i] = s;
        } else if (i >= 16) {
            seq_t s = 0;
            for (int j = 1; j <= 14; ++j)
                s += (seq_t)35 * cache[(size_t)(i - j)];
            s -= (seq_t)630 * cache[(size_t)(i - 15)];
            cache[(size_t)i] = s;
        }
        /* values for i < 15 are already present */
    }
    cache_cap = (size_t)(n + 1);
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
