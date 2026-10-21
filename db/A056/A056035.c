/*
 * A056035  Numbers k such that k^30 == 1 (mod 31^2).
 * https://oeis.org/A056035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-30) + 961 (n > 30), a(1..30) are the solutions * to
 *            k^30 ≡ 1 (mod 961), listed in increasing order. The function *
 *            computes the sequence and memoises it for arbitrary calls
 * Range:     u8 n <= 1, u16 n <= 2044, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
        const seq_t MOD = (seq_t)961U;   /* 31² */

        /* cache for terms already generated */
        static seq_t *cache   = NULL;
        static size_t cap     = 0;   /* allocated size of cache */
        static idx_t  last    = 0;   /* highest index that has been generated (1‑based) */

        /* first 30 terms (solutions <= 961) gathered once */
        static bool   init_done = false;
        static seq_t init30[30];

        /* first time: find the 30 solutions to k^30 ≡ 1 (mod 961) */
        if (!init_done) {
                idx_t cnt = 0;
                for (idx_t k = 1; k <= 961 && cnt < 30; ++k) {
                        if (mod_pow((seq_t)k, 30ULL, MOD) == (seq_t)1)
                                init30[cnt++] = (seq_t)k;
                }
                init_done = true;
        }

        /* allocate cache if not already done */
        if (cache == NULL) {
                cap = 30;
                cache = (seq_t *)malloc(cap * sizeof(seq_t));
                if (!cache) abort();
                for (idx_t i = 0; i < 30; ++i)
                        cache[i] = init30[i];
                last = 30;   /* last generated index (1‑based) */
        }

        /* grow cache if required to hold n entries */
        if (n > cap) {
                size_t newcap = cap;
                while (newcap < n)
                        newcap <<= 1;   /* double until sufficient */
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = newcap;
        }

        /* generate any missing terms up to n */
        if (n > last) {
                for (idx_t i = last + 1; i <= n; ++i) {   /* i is 1‑based */
                        /* recurrence only needed for i > 30 */
                        cache[i - 1] = cache[(i - 30) - 1] + MOD;
                }
                last = n;
        }

        return cache[n - 1];
}
