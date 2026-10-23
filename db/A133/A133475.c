/*
 * A133475  Integers n such that n^3 + n^2 - 9*n + 16 is a square.
 * https://oeis.org/A133475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, full, fini
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Seq: A133475
 * Formula: Integers n such that n^3 + n^2 - 9*n + 16 is a square.
 * Implementation: Perform an incremental scan of all integers, storing the
 * indices that satisfy the condition.  The scan is lazy, only extending as
 * many terms as are requested by the caller.  The search stops when a
 * predetermined high bound is exceeded; if more terms are requested than
 * exist in this bound, the function returns 0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static mutable state */
    static seq_t *cache = NULL;          /* array of found n's         */
    static size_t cache_len = 0;         /* number of stored elements  */
    static size_t cache_cap = 0;         /* current allocation size    */
    static int64_t scan_pos = -10;       /* next integer to test       */
    static const int64_t MAX_SCAN = 2000000; /* upper limit for the scan   */

    /* lazy growth of the cache array */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure -> sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend the cache until it contains at least n elements or
       until the scan reaches its upper bound           */
    while (cache_len < n) {
        if (scan_pos > MAX_SCAN) break; /* no further search possible */

        seq_t cur = (seq_t)scan_pos;
        /* compute expr = n^3 + n^2 - 9*n + 16  */
        seq_t expr = cur * cur * cur + cur * cur - (seq_t)9 * cur + 16;

        if (expr >= 0) {
            seq_t root = floor_sqrt(expr);
            if (root * root == expr) {
                cache[cache_len++] = cur;  /* store found integer */
                if (cache_len == cache_cap) { /* resize if needed */
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;
                    cache = tmp;
                    cache_cap = new_cap;
                }
            }
        }
        scan_pos++; /* advance to the next integer */
    }

    /* If we have enough terms, return the requested one; otherwise
       return 0 as a sentinel value.                        */
    if (cache_len >= n) {
        return cache[n - 1];
    } else {
        return 0;
    }
}
