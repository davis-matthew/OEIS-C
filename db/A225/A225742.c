/*
 * A225742  Numbers >= 10 that are divisible by the sum and difference of
 *          their digits.
 * https://oeis.org/A225742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 2602, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_to_str.h"

static seq_t *cache   = NULL; /* stored terms   */
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of terms   */
static seq_t next_num = 10;    /* next candidate */

/* Grow the cache to hold at least `min_cap` elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Append a new term to the cache. */
static void add_term(seq_t val)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* Generate terms up to the required index `need`. */
/* This function advances `next_num` and fills the cache. */
static void generate_up_to(idx_t need)
{
    /* buffer for decimal representation; numbers in the sequence
       are relatively small, 1024 bytes is more than enough. */
    char buf[1024];
    size_t len = 0;
    while ((idx_t)cache_len < need) {
        seq_t n = next_num;

        /* Convert to decimal string to obtain digits and msd. */
        seq_to_str(n, buf, &len);
        if (len == 0) { /* safety, should not happen */
            next_num = next_num + 1;
            continue;
        }

        int sum = 0;
        int msd = buf[0] - '0';
        for (size_t i = 0; i < len; ++i) {
            sum += buf[i] - '0';
        }

        int diff = 2 * msd - sum;    /* msd - (sum - msd) */
        if (diff != 0) {
            seq_t sum_seq = (seq_t)sum;
            seq_t diff_seq = (seq_t)(diff > 0 ? diff : -diff);
            if ((n % sum_seq == 0) && (n % diff_seq == 0)) {
                add_term(n);
            }
        }

        next_num = next_num + 1;
    }
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for indices < 1 */
    if ((idx_t)cache_len < n) generate_up_to(n);
    return cache[n - 1];
}
