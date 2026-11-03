/*
 * A068403  Numbers k such that sigma(k) > 3*k.
 * https://oeis.org/A068403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 713, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9677 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A068403: numbers k such that σ(k) > 3·k.
   The sequence is generated on demand by a linear search.
   Each found value is cached, so any subsequent call for a
   larger index reuses the previously computed terms. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* 1‑based offset: a(1) is the first element returned. */
    if (n == 0)
        return (seq_t)0;          /* undefined for this sequence */

    static seq_t *cache = NULL;
    static size_t cap  = 0, len = 0;   /* cache capacity and length   */
    static idx_t next  = 1;            /* next integer to test       */

    /* Generate terms until the cache contains at least n values. */
    while (len < n)
    {
        /* Search for the next qualifying integer. */
        for (;;)
        {
            seq_t sigma = sigma_of(next);
            seq_t thr   = (seq_t)next * 3;

            if (sigma > thr)
            {
                /* Grow the cache if needed. */
                if (len + 1 > cap)
                {
                    size_t newcap = cap ? cap * 2 : 64;
                    seq_t *tmp = mem_resize(seq_t, cache, newcap);
                    if (!tmp)
                        abort();   /* unrecoverable allocation failure */
                    cache = tmp;
                    cap   = newcap;
                }

                cache[len++] = (seq_t)next;
                next++;       /* move to the next integer to test */
                break;        /* found a term – proceed to outer loop */
            }
            next++;           /* continue searching */
        }
    }
    return cache[n - 1];
}
