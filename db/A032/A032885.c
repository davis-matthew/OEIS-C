/*
 * A032885  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(0)
 *          < d(1) > d(2) < ...
 * https://oeis.org/A032885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032885: numbers whose base‑6 digits alternate
   d(0) < d(1) > d(2) < d(3) …            */
/* Implementation: for a given n, iterate through positive integers,
   test the alternating property on their base‑6 representation,
   and keep a cache to answer repeated or out‑of‑order requests. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cache[0] unused */
static size_t  cache_cap = 0;
static idx_t   cache_cnt = 0;        /* number of terms stored (cache_cnt >= 0) */
static seq_t  next_candidate = 1;    /* next integer to test */

/* Ensure cache can hold terms up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) exit(1);   /* allocation failure */
    cache     = newp;
    cache_cap = new_cap;
}

/* Test whether val has alternating inequalities in base‑6. */
static bool
good(seq_t val)
{
    seq_t v = val;
    seq_t prev = v % 6;
    v /= 6;
    int dir = 1;          /* expect next digit > prev for first comparison */
    while (v > 0) {
        seq_t cur = v % 6;
        v /= 6;
        if (dir == 1) {
            if (!(cur > prev)) return false;
        } else {
            if (!(cur < prev)) return false;
        }
        prev = cur;
        dir  = -dir;
    }
    return true;
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but safe return */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }
    if (cache_cnt < n) {
        while (cache_cnt < n) {
            if (good(next_candidate)) {
                ++cache_cnt;
                ensure_cache(cache_cnt);
                cache[cache_cnt] = next_candidate;
            }
            ++next_candidate;
        }
    }
    return cache[n];
}
