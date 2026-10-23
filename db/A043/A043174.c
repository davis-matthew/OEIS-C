/*
 * A043174  Numbers k such that 4 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043174: numbers k where the digits 4 and 5 appear
   adjacent in base‑8 representation of k but not in k‑1.
   The function searches sequentially, caching found terms so
   that any call order is handled efficiently. */

#ifdef __cplusplus
#include <vector>
#else
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#endif

/* Forward declaration of helper */
static bool has_adj45_54(seq_t x);

/* --------------------------- C implementation --------------------------- */
#ifndef __cplusplus
/* Dynamic cache for C */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t   next_cand = 1;   /* next integer to test */

/* Ensure cache has space for at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Check if x has adjacent digits 4 and 5 (in any order) in base‑8. */
static bool has_adj45_54(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;
    while (x > 0) {
        int cur = (int)(x % 8);
        if (prev != -1 && ((cur == 4 && prev == 5) || (cur == 5 && prev == 4)))
            return true;
        prev = cur;
        x /= 8;
    }
    return false;
}
#endif

/* -------------------------- Common part ------------------------------ */
#ifndef __cplusplus
/* Seq_t in C must be initialized when stored; default construction
   via realloc is sufficient for trivial types. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive, offset is 1 */

    while (cache_size < (size_t)n) {
        seq_t cur = next_cand;
        bool cur_has = has_adj45_54(cur);
        bool prev_has = has_adj45_54(cur - 1);

        if (cur_has && !prev_has) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = cur;
        }
        next_cand++;
    }
    return cache[(size_t)(n - 1)];
}
#else
/* C++ implementation using std::vector to avoid custom allocation. */
static std::vector<seq_t> cache;
static idx_t next_cand = 1;

static bool has_adj45_54(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;
    while (x > 0) {
        int cur = (int)(x % 8);
        if (prev != -1 && ((cur == 4 && prev == 5) || (cur == 5 && prev == 4)))
            return true;
        prev = cur;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return seq_t{};               /* defensive, offset is 1 */

    while (cache.size() < (size_t)n) {
        seq_t cur = next_cand;
        bool cur_has = has_adj45_54(cur);
        bool prev_has = has_adj45_54(cur - 1);

        if (cur_has && !prev_has) {
            cache.push_back(cur);
        }
        ++next_cand;
    }
    return cache[(size_t)(n - 1)];
}
#endif
