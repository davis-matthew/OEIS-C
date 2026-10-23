/*
 * A138708  Numbers m such that A138707(m) = 1.
 * https://oeis.org/A138708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer that is odd and
 * every non‑zero decimal digit of a(n) is coprime with a(n).
 * This is OEIS A138708. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cnt  = 0;   /* number of stored terms    */
    static size_t cache_cap  = 0;   /* capacity of cache        */
    static idx_t  last_checked = 0; /* largest integer inspected */

    /* Return cached value if already available */
    if (cache_cnt >= (size_t)n)
        return cache[n - 1];

    /* Initialise cache lazily */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Find terms until we have at least n of them */
    if (cache_cnt < (size_t)n) {
        idx_t cand;
        for (cand = last_checked + 1; cache_cnt < (size_t)n; ++cand) {
            /* Only odd candidates are possible */
            if ((cand & 1) == 0)
                continue;

            seq_t num = (seq_t)cand;
            bool ok = true;
            idx_t tmp = cand;
            while (tmp) {
                unsigned digit = (unsigned)(tmp % 10);
                tmp /= 10;
                if (digit == 0)
                    continue;                    /* zero digits are ignored   */
                if (gcd(num, (seq_t)digit) != (seq_t)1) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                if (cache_cnt == cache_cap) {   /* grow cache if needed */
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = num;
            }
        }
        last_checked = cand - 1;              /* cand was incremented one step beyond last test */
    }

    return cache[n - 1];
}
