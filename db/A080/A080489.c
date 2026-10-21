/*
 * A080489  a(1) =4, a(n) = smallest multiple of a(n-1) (not equal to
 *          10^k*a(n-1)) obtained by inserting digits anywhere in a(n-1).
 * https://oeis.org/A080489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   get_digits, is_subsequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=4; a(n)=smallest multiple of a(n-1) (not 10^k·a(n-1))
   obtained by inserting digits anywhere in a(n‑1).  */

/* Standard headers and utilities */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/is_subsequence.h"

/* Max number of decimal digits we expect to handle for any n <= 10000
   (kept generously large). */
#define MAX_DIGITS 20000

/* Compute a(n) from a(n‑1). */
static seq_t find_next(seq_t prev)
{
    static uint8_t prev_buf[MAX_DIGITS];
    static uint8_t cand_buf[MAX_DIGITS];
    static uint8_t prev_rev[MAX_DIGITS];
    static uint8_t cand_rev[MAX_DIGITS];
    size_t prev_len, cand_len;

    /* Obtain digits of the previous term (LSB‑first) */
    {
        size_t len = MAX_DIGITS;
        get_digits(prev, prev_buf, &len);
        prev_len = len;
    }

    /* Reverse to MSB‑first order */
    for (size_t i = 0; i < prev_len; ++i)
        prev_rev[i] = prev_buf[prev_len - 1 - i];

    /* Search multiples of prev; start from 2 */
    for (uint64_t k = 2;; ++k) {
        seq_t cand = (seq_t)k * prev;

        /* Obtain digits of the candidate (LSB‑first) */
        {
            size_t len = MAX_DIGITS;
            get_digits(cand, cand_buf, &len);
            cand_len = len;
        }

        /* Reverse to MSB‑first order */
        for (size_t i = 0; i < cand_len; ++i)
            cand_rev[i] = cand_buf[cand_len - 1 - i];

        /* reject trivial 10^t·prev cases */
        if (cand_len > prev_len) {
            bool all_zero = true;
            size_t i;
            for (i = 0; i < prev_len; ++i) {
                if (cand_rev[i] != prev_rev[i]) { all_zero = false; break; }
            }
            if (all_zero) {
                for (; i < cand_len; ++i) {
                    if (cand_rev[i] != 0) { all_zero = false; break; }
                }
            }
            if (all_zero) continue;
        }

        /* check subsequence property */
        if (is_subsequence(prev_rev, prev_len, cand_rev, cand_len))
            return cand;
    }
}

/* Memoised cache (index 0 unused). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Public function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for 0 or negative indices */

    /* initialise cache */
    if (cache_cap == 0) {
        size_t init_cap = 2;          /* indices 0 and 1 */
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[1] = (seq_t)4;          /* a(1) = 4 */
        cache_cap = 1;                /* terms up to 1 are cached */
    }

    /* extend cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n + 1;   /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (size_t i = cache_cap + 1; i <= (size_t)n; ++i) {
            cache[i] = find_next(cache[i - 1]);
        }
        cache_cap = (size_t)n;
    }

    return cache[(size_t)n];
}
