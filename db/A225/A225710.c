/*
 * A225710  Composite squarefree numbers n such that p(i)-10 divides n+10,
 *          where p(i) are the prime factors of n.
 * https://oeis.org/A225710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates composite, square‑free integers n such that for each
 * prime factor p of n the difference p-10 divides n+10. It stores the results
 * in a dynamically grown cache so that any call a(n) runs in O(1) after the
 * sequence has been generated once. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Cache of generated terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;

    /* The smallest composite square‑free integer is 14, so we start testing
     * from 12 (the first two undershoots are never valid). */
    static seq_t last = 12;

    /* Generate terms until we have at least n of them */
    while (len < n)
    {
        /* The algorithm operates on 64‑bit values when factoring, so
         * we assume all generated terms fit into a uint64_t; this is
         * true for the first 10 000 terms. */
        uint64_t m64 = (uint64_t)last;

        uint64_t primes[64];
        uint64_t exps[64];
        size_t   cnt = 0;
        factor_u64(m64, primes, exps, &cnt);

        if (cnt >= 2) /* composite */
        {
            bool ok = true;

            for (size_t i = 0; i < cnt && ok; ++i)
            {
                if (exps[i] != 1)      /* not square‑free */
                {
                    ok = false;
                    break;
                }

                uint64_t p = primes[i];
                uint64_t d = (p > 10) ? p - 10 : 10 - p;

                if (d == 0)            /* cannot happen (10 is not prime) */
                {
                    ok = false;
                    break;
                }

                seq_t d_seq = (seq_t)d;
                /* Check (last + 10) % d == 0 */
                seq_t mod_val = (last + (seq_t)10) % d_seq;
                if (mod_val != 0)
                {
                    ok = false;
                    break;
                }
            }

            if (ok) /* accepted term */
            {
                if (len == cap)
                {
                    size_t newcap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp) /* allocation failure – abort */
                        return 0;
                    cache = tmp;
                    cap = newcap;
                }
                cache[len++] = last;
            }
        }
        ++last; /* next candidate */
    }

    return cache[(size_t)(n - 1)];
}
