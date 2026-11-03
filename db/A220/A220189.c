/*
 * A220189  If n>=3, a(n) = n^(a(n-1)/(n-1)), else a(2)=2^2.
 * https://oeis.org/A220189
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(a(n-1)/(n-1)) with a(2)=2^2
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 5 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0 .. cache_size-1]   */
    static idx_t cache_size = 0;  /* allocated length           */
    static idx_t last = 1;        /* highest index computed     */

    if (n < 2)  /* offset starts at 2 – undefined below */
        return (seq_t)0;

    /* Ensure the cache is large enough to hold index n   */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1) * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp)   /* allocation failed – return 0 as error code */
            return (seq_t)0;
        /* Initialize the new entries to 0 (default constructor
           usually does that for arbitrary‑precision types).         */
        for (idx_t i = cache_size; i <= n; ++i)
            tmp[i] = (seq_t)0;
        cache      = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        if (k == 2) {
            cache[2] = (seq_t)4; /* 2^2 */
        } else {
            seq_t prev   = cache[k - 1];
            seq_t exp    = prev / (seq_t)(k - 1);  /* integer division */
            cache[k] = ipow((seq_t)k, exp);
        }
    }
    if (n > last)
        last = n;

    return cache[n];
}
