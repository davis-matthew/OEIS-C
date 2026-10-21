/*
 * A239937  Numbers k such that digsum(k^2) > digsum((k+1)^2).
 * https://oeis.org/A239937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 651, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239937: numbers k such that digsum(k^2) > digsum((k+1)^2).
   The function returns the n-th such k (offset 1).  Results are
   memoised in an array of 64‑bit indices to avoid overflow of seq_t
   when it is too small for the needed k-values.  */
#include <stdlib.h>
#include "../utils/digit_sum.h"

static idx_t *cache   = NULL;   /* stored indices k (idx_t)          */
static size_t cached  = 0;      /* how many k’s have been stored     */
static size_t cap     = 0;      /* current capacity of cache array   */
static idx_t next_k   = 0;      /* next candidate k to examine      */

seq_t a(idx_t n)
{
    if (n <= 0) return 0;               /* unused, offset 1              */
    size_t need = (size_t)n;            /* number of terms required      */

    /* grow storage while we need more terms */
    while (cached < need) {
        /* examine next integer k */
        ++next_k;
        idx_t kp1   = next_k + 1;
        seq_t sq_k  = (seq_t)next_k  * (seq_t)next_k;
        seq_t sq_kp = (seq_t)kp1 * (seq_t)kp1;

        if (digit_sum(sq_k) > digit_sum(sq_kp)) {
            /* ensure capacity */
            if (cached == cap) {
                size_t new_cap = cap ? cap << 1 : 16;
                idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[cached++] = next_k;   /* store found k         */
        }
    }

    /* return the n‑th element, casting to seq_t.  The value must fit. */
    return (seq_t)cache[n - 1];
}
