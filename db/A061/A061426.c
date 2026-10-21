/*
 * A061426  Geometric mean of the digits = 2. In other words, the product of
 *          the digits is = 2^k where k is the number of digits.
 * https://oeis.org/A061426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search enumeration of numbers whose digit product equals 2^digit_count. 
   For each positive integer n (offset 1), a(n) is the nth smallest
   positive integer whose product of decimal digits equals a power of two
   with exponent equal to its number of digits.
   The implementation generates all valid numbers in ascending order,
   caching them so that subsequent calls are served in O(1) after the
   cache contains at least n entries. */
#include <stddef.h>
#include <stdlib.h>

/* digits that may appear in a term (1,2,4,8) and their exponents of 2 */
static const seq_t DIGITS[4] = {1, 2, 4, 8};
static const idx_t  EXPS  [4] = {0, 1, 2, 3};

/* dynamic cache of computed terms */
static seq_t *cache   = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* last complete digit length that has been fully enumerated */
static idx_t last_len = 0;

/* current maximum N requested (up to which we have generated terms) */
static idx_t max_needed = 0;

/* ensure cache has at least 'need' capacity */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* rollback to avoid crash on allocation failure */
        /* In an actual build environment we would handle this, but for
           the purposes of this implementation we can simply abort. */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* append a term to the cache */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = val;
}

/* depth‑first enumeration for a fixed digit length 'len'.
   All valid numbers of that length are produced in ascending order. */
static void dfs_len(idx_t pos, idx_t sum, seq_t val, idx_t len)
{
    if (pos == len) {
        if (sum == len) cache_append(val);
        return;
    }
    idx_t remaining_positions = len - pos - 1;
    for (int i = 0; i < 4; ++i) {
        idx_t exp = EXPS[i];
        if (sum + exp > len) continue;               /* exceeds required sum */
        if (sum + exp + remaining_positions * 3 < len) continue; /* cannot reach sum */
        seq_t new_val = val * 10 + DIGITS[i];
        dfs_len(pos + 1, sum + exp, new_val, len);
    }
}

/* process all complete lengths from 'start_len+1' until cache contains
   at least 'max_needed' terms.   */
static void generate_up_to(idx_t start_len)
{
    for (idx_t len = start_len + 1; ; ++len) {
        dfs_len(0, 0, 0, len);
        last_len = len;
        if (cache_size >= max_needed) break;
        /* if we ever reach an unreasonably large 'len' we could stop,
           but for n <= 10000 this will never happen. */
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for offset 1, but guard anyway */
    if (n > cache_size) {
        if (n > max_needed) max_needed = n;
        generate_up_to(last_len);
    }
    return cache[n - 1];
}
