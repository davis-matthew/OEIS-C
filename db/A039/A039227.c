/*
 * A039227  Numbers whose base-12 representation has the same number of 1's
 *          and 9's.
 * https://oeis.org/A039227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑12 representation has equal numbers of 1's and 9's
   The function a(n) returns the n‑th such number (a(1)=0).  The implementation
   lazily generates terms on demand and caches them for efficient repeated
   queries.  The check for the condition is done digit‑by‑digit using only
   integer arithmetic, fulfilling the constraints of the required build
   environments. */
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static seq_t last_value = 0;

static bool is_ok(seq_t x)
{
    if (x == 0)
        return true;
    unsigned int c1 = 0, c9 = 0;
    while (x > 0) {
        seq_t d = x % 12;
        if (d == 1)
            ++c1;
        else if (d == 9)
            ++c9;
        x = x / 12;
    }
    return c1 == c9;
}

static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (target == 0)
        return 0;           /* undefined for n<1, but keeps code safe */

    /* initialize cache with the first term 0 */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)0;
        cache_len = 1;
        last_value = (seq_t)0;
    }

    while (cache_len < target) {
        seq_t cand = last_value + (seq_t)1;
        for (;;) {
            if (is_ok(cand)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
                last_value = cand;
                break;
            }
            cand++;
        }
    }
    return cache[target - 1];
}
