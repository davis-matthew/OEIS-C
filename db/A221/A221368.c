/*
 * A221368  A two-digit Look-and-Say sequence starting with 12: each term
 *          summarizes the increasing two-digit substrings of the previous
 *          term.
 * https://oeis.org/A221368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   append_count, produce_next, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221368: Two‑digit look‑and‑say sequence starting with 12.
   a(0) = 12.
   a(n) (n > 0) is obtained by taking all two‑digit substrings of a(n‑1),
   counting their frequencies, sorting the substrings in increasing order,
   and concatenating for each substring “<count><substring>”. */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/append_count.h"
#include "../utils/produce_next.h"

static seq_t *cache = NULL;          /* memoized terms */
static size_t cache_cap = 0;          /* allocated cache capacity */
static size_t cache_len = 0;          /* number of cached terms */

/* Current term as a NUL‑terminated string. */
static char *cur = NULL;
static size_t cur_len = 0;            /* length without NUL */

/* Temporary buffer to build the next term. */
static char *next_buf = NULL;
static size_t next_cap = 0;

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute terms up to n. */
    while (cache_len <= (size_t)n) {
        if (cache_len == 0) {
            /* a(0) = 12 */
            cur = (char *)malloc(3);          /* "12" + '\0' */
            strcpy(cur, "12");
            cur_len = 2;

            if (cache_cap == 0) {
                cache_cap = 8;
                cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = seq_from_str(cur);
        } else {
            size_t new_len;
            produce_next(cur, cur_len, &next_buf, &new_len, &next_cap);

            free(cur);                /* free old string */
            cur = next_buf;           /* new string becomes current */
            cur_len = new_len;
            next_buf = NULL;
            next_cap = 0;

            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = seq_from_str(cur);  /* cache new value */
        }
    }
    return cache[(size_t)n];
}
