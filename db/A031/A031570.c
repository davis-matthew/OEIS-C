/*
 * A031570  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 72.
 * https://oeis.org/A031570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑trivial sequence: numbers k such that continued fraction for sqrt(k)
   has even period and the central term (after deleting the last term
   2*a0) equals 72.  Computed by incremental search. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/int_sqrt.h"

#define MAX_PERIOD 30000
#define CACHE_CAP 200000

/* Working buffer for a single continued‑fraction expansion */
static uint32_t cf[MAX_PERIOD];

/* Cache of found terms */
static seq_t *seq_cache = NULL;
static idx_t  seq_used  = 0;     /* number of terms currently stored */
static idx_t  next_k    = 2;     /* next integer to test (global) */

/* Helper to allocate cache when needed */
static void ensure_cache_cap(void)
{
    static size_t cap = 0;
    if (seq_cache == NULL || seq_used == cap)
    {
        size_t new_cap = cap == 0 ? 64 : cap * 2;
        while (new_cap < CACHE_CAP) new_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }
}

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (seq_cache == NULL) ensure_cache_cap();

    /* Find enough terms */
    while (seq_used < n)
    {
        idx_t k = next_k++;

        /* Skip perfect squares */
        idx_t a0 = int_sqrt(k);
        if (a0 * a0 == k) continue;

        idx_t m = 0;
        idx_t d = 1;
        idx_t a_cur = a0;
        int period = 0;

        /* Generate the continued‑fraction until a_cur == 2*a0 */
        while (1)
        {
            m = d * a_cur - m;
            d = (k - m * m) / d;
            a_cur = (a0 + m) / d;
            cf[period++] = (uint32_t)a_cur;
            if (a_cur == 2 * a0) break;
            if (period >= MAX_PERIOD) break;   /* safeguard */
        }

        /* Even period?  (include the last term 2*a0) */
        if ((period & 1) == 0 && period >= 2)
        {
            int centralIdx = (period - 2) / 2;   /* after deleting last term */
            if (cf[centralIdx] == 72)
            {
                /* Store term */
                ensure_cache_cap();
                seq_cache[seq_used++] = (seq_t)k;
            }
        }
    }

    return seq_cache[n - 1];
}
