/*
 * A043893  Numbers k such that 2 and 3 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043893: Numbers k such that the digits 2 and 3 occur
   adjacent (either order) in the base‑5 representation of k but
   not in that of k+1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoised terms */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* capacity of the array */
static seq_t next_k = 0;      /* last value of k examined */

/* Tests whether x in base‑5 contains adjacent digits 2 and 3
   (in either order). */
static bool has_adjacent_23(seq_t x)
{
    seq_t prev = 5;   /* "none" marker, larger than any base‑5 digit */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)5;
        if ((prev == (seq_t)3 && d == (seq_t)2) ||
            (prev == (seq_t)2 && d == (seq_t)3))
            return true;
        prev = d;
        x /= (seq_t)5;
    }
    return false;
}

/* Ensure the cache has room for at least 'need' entries. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);         /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function computing a(n).  It is memoised so that calls with
   arbitrary n arrive in any order. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* offset is 1, undefined for 0 */

    size_t need = (size_t)n;
    grow_cache(need);
    while (cache_len < need) {
        next_k = next_k + (seq_t)1;
        if (has_adjacent_23(next_k) && !has_adjacent_23(next_k + (seq_t)1)) {
            cache[cache_len++] = next_k;
        }
    }
    return cache[need - 1];
}
