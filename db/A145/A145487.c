/*
 * A145487  Numbers k such that 6k+5 is prime and 12k+5 is also prime.
 * https://oeis.org/A145487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = k such that 6k+5 and 12k+5 are both prime (OEIS A145487)
 * Range:     u8 n <= 65, u16 n <= 4760, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 8072, big any n
 * Validated: standard, pass, 79 terms through n = 9885 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, etc.  The function returns the n‑th term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* stored k values */
    static idx_t  len     = 0;      /* number of terms stored   */
    static idx_t  cap     = 0;      /* current capacity         */
    static uint64_t next_k = 0;     /* next candidate k to test */

    /* grow the cache when necessary */
    if (len == cap) {
        idx_t newcap = (cap) ? cap * 2 : 4;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)newcap);
        if (!tmp) abort();               /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        uint64_t k = next_k++;          /* candidate k */

        /* test primality of 6k+5 and 12k+5 */
        bool ok = is_prime((idx_t)(6 * k + 5)) &&
                  is_prime((idx_t)(12 * k + 5));

        if (ok) {
            cache[len++] = (seq_t)k;    /* store the k value */
        }
    }

    return cache[n - 1];
}
