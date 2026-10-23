/*
 * A039239  Numbers whose base-12 representation has the same number of 3's
 *          and 4's.
 * https://oeis.org/A039239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non-negative integer whose base‑12
 *            representation contains the same number of 3’s and 4’s (OEIS
 *            A039239). The function computes on the fly, memoising the
 *            results in a static table so that repeated calls with arbitrary
 *            indices are answered in O(1) after the needed terms have been
 *            g...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper to decide if a number has equal count of 3 and 4 digits in base‑12. */
static bool equal34(seq_t x)
{
    if (x == (seq_t)0)          /* 0 has 0 3’s and 0 4’s */
        return true;
    int c3 = 0, c4 = 0;
    seq_t t = x;
    while (t != (seq_t)0) {
        int d = (int)(t % (seq_t)12);  /* digit 0…11 */
        if (d == 3) c3++;
        else if (d == 4) c4++;
        t /= (seq_t)12;
    }
    return c3 == c4;
}

#ifdef __cplusplus
/* For C++ builds (arbitrary‑precision types).  Use a std::vector for storage. */
#include <vector>
static std::vector<seq_t> cache;      /* cache of computed values */
static seq_t last_checked = 0;        /* integer just tested for the next value */

/* Ensure that enough terms up to n are generated. */
static void ensure_terms(idx_t n)
{
    if (n <= cache.size())
        return;
    while (cache.size() < (size_t)n) {
        if (equal34(last_checked))
            cache.push_back(last_checked);
        last_checked++;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    ensure_terms(n);
    return cache[(size_t)(n - 1)];
}

#else
/* For plain C builds (unsigned integer types).  Use a dynamic array. */
#include <stdlib.h>
static seq_t *cache = NULL;   /* dynamic array holding computed terms   */
static size_t cache_cap = 0;  /* allocated capacity in elements          */
static size_t cache_len = 0;  /* number of valid elements                */
static seq_t last_checked = 0;  /* integer just tested for the next value */

/* Grow the cache if necessary. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that at least n terms are generated. */
static void ensure_terms(idx_t n)
{
    if (n <= (idx_t)cache_len)
        return;
    while (cache_len < (size_t)n) {
        if (equal34(last_checked)) {
            if (cache_len >= cache_cap)
                grow_cache();
            cache[cache_len++] = last_checked;
        }
        last_checked++;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    ensure_terms(n);
    return cache[(size_t)(n - 1)];
}
#endif
