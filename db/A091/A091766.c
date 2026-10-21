/*
 * A091766  For n > 2, let m be the least number that cannot be expressed as
 *          a(i), a(i)+a(j), or a(i)-a(j) with i, j < n. Then a(n) = a(n-1)+m.
 * https://oeis.org/A091766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091766
   a(0) = 1, a(1) = 2, a(2) = 6
   For n > 2, let m be the least positive integer that cannot be
   expressed as a(i), a(i)+a(j), or a(i)-a(j) with i, j < n.
   Then a(n) = a(n-1)+m.
   The implementation keeps a cache of already computed terms and
   a bitset of all numbers that are representable with at most two
   distinct weights.  Each call expands the cache if necessary, so the
   function works correctly for calls in any order.  The code is C11 /
   C++17 compatible. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Shared mutable state ----------------------------------------------------- */
static seq_t *cache = NULL;      /* a(0)...a(n) */
static size_t cache_len = 0;     /* number of stored values */
static size_t cache_cap = 0;     /* cache capacity in elements */

static uint8_t *seen = NULL;     /* bitset of representable numbers */
static size_t seen_bytes = 0;    /* capacity in bytes of seen */
static seq_t next_missing = 1;   /* next candidate for the missing number */

/* Ensure the cache has room for at least `need` elements.  */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap << 1 : 16;
    while (cap < need)
        cap <<= 1;
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache_cap = cap;
}

/* Ensure the bitset can hold bits up to value `val`. */
static void ensure_seen_for_value(seq_t val)
{
    size_t v = (size_t)val;
    size_t byte_index = v >> 3;
    if (byte_index < seen_bytes)
        return;
    size_t cap = seen_bytes ? seen_bytes << 1 : 128;
    while (cap <= byte_index)
        cap <<= 1;
    seen = (uint8_t *)realloc(seen, cap);
    memset(seen + seen_bytes, 0, cap - seen_bytes);
    seen_bytes = cap;
}

/* Mark `val` as representable. */
static inline void set_seen(seq_t val)
{
    size_t v = (size_t)val;
    size_t byte_idx = v >> 3;
    uint8_t mask = 1 << (v & 7);

    if (byte_idx >= seen_bytes) {
        size_t cap = seen_bytes ? seen_bytes << 1 : 128;
        while (cap <= byte_idx)
            cap <<= 1;
        seen = (uint8_t *)realloc(seen, cap);
        memset(seen + seen_bytes, 0, cap - seen_bytes);
        seen_bytes = cap;
    }
    seen[byte_idx] |= mask;
}

/* Test whether `val` is currently marked as representable. */
static inline int is_seen(seq_t val)
{
    size_t v = (size_t)val;
    size_t byte_idx = v >> 3;
    if (byte_idx >= seen_bytes)
        return 0;
    uint8_t mask = 1 << (v & 7);
    return (seen[byte_idx] & mask) != 0;
}

/* Initialise the sequence with the first three terms. */
static void init_sequence(void)
{
    if (cache_len > 0)
        return;                      /* already initialised */

    ensure_cache(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)6;
    cache_len = 3;

    /* Mark all representable numbers from the first three terms.
       Sums of distinct terms only. */
    for (idx_t i = 0; i < 3; ++i) {
        set_seen(cache[i]);                 /* a(i) itself */
    }
    for (idx_t i = 1; i < 3; ++i) {
        for (idx_t j = 0; j < i; ++j) {
            seq_t sum = cache[i] + cache[j];
            set_seen(sum);
            if (cache[i] > cache[j])
                set_seen(cache[i] - cache[j]);
        }
    }

    /* Find the first missing positive integer. */
    while (is_seen(next_missing))
        ++next_missing;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_sequence();

    /* Grow the cache up to index n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 3)                     /* first three terms already set */
            continue;

        /* Determine m for the current index. */
        seq_t m = next_missing;
        seq_t term = cache[i - 1] + m;

        ensure_cache(i + 1);
        cache[i] = term;
        cache_len = i + 1;

        /* Update representable numbers with the new term. */
        set_seen(term);                  /* a(i) itself */
        for (idx_t j = 0; j < i; ++j) {
            seq_t sum = term + cache[j];
            set_seen(sum);
            if (term > cache[j])
                set_seen(term - cache[j]);
        }

        /* Advance next_missing to the next unused integer. */
        while (is_seen(next_missing))
            ++next_missing;
    }

    return cache[n];
}
