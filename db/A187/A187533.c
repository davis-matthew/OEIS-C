/*
 * A187533  Numbers divisible by at least five of their digits, different and
 *          >1.
 * https://oeis.org/A187533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers divisible by at least five of their OLD distinct decimal digits,
   digits >1.  The function returns the n‑th such number (n≥1). */
/* This implementation generates the numbers in ascending order,
   caching them so that subsequent calls do not recompute earlier terms. */

#include <stdlib.h>
#include <stdbool.h>

/* both seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    /* Ensure we have the first n terms cached. */
    static seq_t *cache      = NULL;   /* array of found numbers */
    static size_t cache_len  = 0;      /* number of terms currently stored   */
    static size_t cache_cap  = 0;      /* allocated capacity                */
    static uint64_t next_to_check = 1; /* next integer to test              */

    if (n == 0)                     /* offset is 1; return 0 for safety */
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (cache_len < (size_t)n) {
        /* Generate terms until we have at least n entries. */
        while (cache_len < (size_t)n) {
            /* Test whether next_to_check qualifies. */
            uint64_t num = next_to_check;
            bool seen[10] = {0};
            size_t diffcnt = 0;
            uint64_t tmp = num;

            while (tmp > 0 && diffcnt < 5) {
                unsigned d = (unsigned)(tmp % 10);
                tmp /= 10;

                if (d > 1 && !seen[d]) {
                    seen[d] = true;
                    if (num % d == 0)
                        ++diffcnt;
                }
            }

            if (diffcnt >= 5) {
                /* Store the qualifying number. */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!newp) /* out of memory – abort */
                        abort();
                    cache = newp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)num;
            }

            ++next_to_check; /* advance to the next integer */
        }
    }

    /* Now cache_len ≥ n. Return the requested term. */
    return cache[(size_t)n - 1];
}
