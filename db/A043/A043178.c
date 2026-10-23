/*
 * A043178  Numbers k such that 5 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043178: numbers k such that 5 and 7 occur juxtaposed in
   the base‑8 representation of k but not in that of k‑1.  The code
   searches for terms by scanning octal digits. */

#include <stdbool.h>
#include <stdlib.h>

static bool has57or75(seq_t v)
{
    if (v == 0) return false;
    seq_t cur = v;
    while (cur >= 8) {
        seq_t l = cur % 8;          /* lower digit  */
        cur /= 8;
        seq_t h = cur % 8;          /* higher digit */
        if ((h == 5 && l == 7) || (h == 7 && l == 5))
            return true;
    }
    return false;
}

static seq_t *cache = NULL;      /* stored terms                                        */
static size_t cache_len = 0;     /* number of terms cached                             */
static size_t cache_cap = 0;     /* current array capacity                             */
static seq_t next_k = 1;         /* smallest integer to test for the next term         */

seq_t a(idx_t n)
{
    /* Fill cache until it contains at least the n‑th term. */
    while (cache_len < n) {
        /* Find the next k satisfying the condition. */
        while (true) {
            if (has57or75(next_k) && !has57or75(next_k - 1)) {
                /* Store the new term. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* In practice we never run out of memory on the
                           test data; on failure return 0. */
                        return 0;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_k;
                ++next_k;            /* advance to the next candidate          */
                break;                /* outer loop will continue searching      */
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
