/*
 * A230862  Start with 0; each term when spelled out must start with the last
 *          letter of the previous term and must be the smallest such number
 *          that is greater than the previous term.
 * https://oeis.org/A230862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, word
 * Formula:   a(n) is the smallest integer > a(n‑1) whose English name begins
 *            * with the last letter of the English name of a(n‑1). The first
 *            term is * a(0)=0. This implementation generates the terms by
 *            iterative search * and memoises them so that any sequence of
 *            calls is supported
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   group_last_letter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/group_last_letter.h"

/* first/last letters of number words 0–19, tens, and group names */
static const char FIRST_SMALL[20] = {
    '?', 'o', 't', 't', 'f', 'f', 's', 's', 'e', 'n',
    't', 'e', 't', 't', 'f', 'f', 's', 's', 'e', 'n'
};

static const char LAST_SMALL[20] = {
    '?', 'e', 'o', 'e', 'r', 'e', 'x', 'n', 't', 'e',
    'n', 'n', 'e', 'n', 'n', 'n', 'n', 'n', 'n', 'n'
};

static const char FIRST_TENS[10] = {
    '?', '?', 't', 't', 'f', 'f', 's', 's', 'e', 'n'
};

static const char LAST_TENS[10] = {
    '?', '?', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y'
};

/* first letter of a number < 1000 */
static inline char first_letter_small_uint64(uint64_t n)
{
    if (n < 20) return FIRST_SMALL[(int)n];
    if (n < 100) return FIRST_TENS[(int)(n / 10)];
    return FIRST_SMALL[(int)(n / 100)];
}

/* first letter of any seq_t number */
static inline char first_letter_any(seq_t n)
{
    if (n == (seq_t)0) return 'z';
    seq_t t = n;
    while (t >= (seq_t)1000) t /= (seq_t)1000;
    return first_letter_small_uint64((uint64_t)t);
}

/* first letter of a 1–999 number, recursive for last digit handling */
static inline char last_letter_small_uint64(uint64_t n)
{
    if (n < 20) return LAST_SMALL[(int)n];
    if (n < 100) {
        if (n % 10 == 0) return LAST_TENS[(int)(n / 10)];
        return last_letter_small_uint64(n % 10);
    }
    if (n % 100 == 0) return 'd';
    return last_letter_small_uint64(n % 100);
}

/* last letter of any seq_t number */
static inline char last_letter_any(seq_t n)
{
    if (n == (seq_t)0) return 'o';

    seq_t temp = n;
    int group_idx = 0;
    while (temp > 0) {
        uint64_t g = (uint64_t)(temp % (seq_t)1000);
        if (g != 0) {
            if (group_idx == 0)
                return last_letter_small_uint64(g);
            else
                return group_last_letter(group_idx);
        }
        temp /= (seq_t)1000;
        ++group_idx;
    }
    return 'o';   /* defensive */
}

/* memoized cache of generated terms */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;

/* return the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n >= seq_cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = NULL;
        if (seq_cache == NULL) {
            new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else if (new_len > seq_cache_len) {
            new_cache = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        } else {
            new_cache = seq_cache;
        }
        if (!new_cache) return (seq_t)0;  /* allocation failure */

        /* generate terms that are not yet in the cache */
        for (idx_t i = seq_cache_len; i < (idx_t)new_len; ++i) {
            if (i == 0) {
                new_cache[0] = (seq_t)0;
            } else {
                seq_t prev = new_cache[i - 1];
                seq_t cand = prev + (seq_t)1;
                char lastL = last_letter_any(prev);

                /* search forward until first letter matches */
                for (;; ++cand) {
                    if (first_letter_any(cand) == lastL) break;
                }
                new_cache[i] = cand;
            }
        }
        seq_cache = new_cache;
        seq_cache_len = new_len;
    }
    return seq_cache[n];
}
