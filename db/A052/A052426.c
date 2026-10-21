/*
 * A052426  Numbers with no two-digit factors (including itself).
 * https://oeis.org/A052426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th positive integer that has no divisor in the range 10
 *            ≤ d ≤ 99 (including itself). The first 9 terms are 1–9,
 *            afterwards the sequence consists of all integers ≥100 that are
 *            not divisible by any 2‑digit number
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1, return 0 */

    static seq_t *cache = NULL;      /* cached sequence values */
    static size_t cache_size = 0;    /* number of stored terms */
    static size_t cache_cap  = 0;    /* allocated capacity of cache */
    static seq_t last = 0;           /* last integer examined */

    /* Grow the cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        /* Next candidate integer */
        ++last;

        int ok = 0;  /* 1 if candidate qualifies */
        if (last < 10) {
            ok = 1;              /* 1..9 are always accepted */
        } else if (last < 100) {
            ok = 0;              /* 10..99 contain themselves as 2‑digit factors */
        } else {
            ok = 1;
            for (int d = 10; d <= 99; ++d) {
                /* If d is a divisor of last, reject it */
                if ((last % d) == 0) {
                    ok = 0;
                    break;
                }
            }
        }

        if (ok) {
            /* Ensure enough space */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failed – abort to avoid undefined behavior */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last;
        }
    }

    return cache[n - 1];
}
