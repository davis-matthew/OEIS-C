/*
 * A043244  Numbers k such that 3 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adj_38
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Search for numbers k such that the decimal representation of k
   contains the substring “38” or “83”, but the representation of k‑1
   does not.  a(n) is the nth such number (OEIS A043244).
*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adj_38.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for 0, but harmless */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* find next k satisfying the condition */
        while (1)
        {
            if (has_adj_38(next_k) && !has_adj_38(next_k - 1))
            {
                /* ensure capacity */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* allocation failure is ignored in this context */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_k;
                break;
            }
            ++next_k;
        }
        /* advance k for the next potential search */
        ++next_k;
    }

    return cache[n - 1];
}
