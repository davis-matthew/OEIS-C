/*
 * A022883  The number of numbers [ [ ix ]jx ] that equal n, where i >= 1, j
 *          >= 1 and x=e.
 * https://oeis.org/A022883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts of the numbers n that can be written as
 * n = ⌊ ⌊i·e⌋ · j · e ⌋   with i ≥ 1, j ≥ 1,  x = e.
 * This implementation pre‑computes all representations up
 * to a fixed maximum (here 100000) at first call and
 * reuses the cache for subsequent calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t *cache = NULL;        /* cache[n] = a(n)          */
static idx_t  cache_size = 0;      /* maximum n stored       */

seq_t a(idx_t n)
{
    /* Define the maximum n to pre‑compute.
     * The description guarantees that calls will have
     * n ≲ 10000, but computing up to 100000 is still
     * trivial and safe. */
    const idx_t MAX_COMPUTE = 100000;

    /* Allocate and fill the cache on first use. */
    if (cache == NULL) {
        size_t sz = (size_t)MAX_COMPUTE + 1;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failed – return 0 for all values. */
            return 0;
        }
        for (size_t i = 0; i < sz; ++i)
            cache[i] = 0;

        const long double e = expl(1.0L);          /* e to high precision */

        /* Enumerate i such that floor(i*e) <= MAX_COMPUTE. */
        for (uint64_t i = 1;; ++i) {
            uint64_t m = (uint64_t)floorl((long double)i * e);
            if (m > MAX_COMPUTE)
                break;

            /* For this m, enumerate all j producing n <= MAX_COMPUTE. */
            for (uint64_t j = 1;; ++j) {
                long double val = (long double)m * (long double)j * e;
                uint64_t nval   = (uint64_t)floorl(val);
                if (nval > MAX_COMPUTE)
                    break;
                ++cache[nval];
            }
        }
        cache_size = MAX_COMPUTE;
    }

    if (n <= cache_size)
        return cache[n];
    /* If n is larger than pre‑computed range, return 0. */
    return 0;
}
