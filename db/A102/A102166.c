/*
 * A102166  Numbers n such that 2*n^2 + 11*n + 101 is prime.
 * https://oeis.org/A102166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th non‑negative integer m with 2*m^2 + 11*m + 101 a
 *            prime
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;  /* stored n-values (a(1) = cache[0])   */
    static idx_t  cache_cap = 0;     /* allocated capacity                    */
    static idx_t  cache_sz  = 0;     /* number of terms stored               */
    static idx_t  next_cand = 0;     /* next integer m to test              */

    /* keep generating terms until we have at least n of them */
    while (cache_sz < n)
    {
        idx_t cand = next_cand++;                /* candidate m                 */
        uint64_t m   = (uint64_t)cand;           /* promote to 64‑bit            */
        uint64_t val = 2ULL * m * m + 11ULL * m + 101ULL; /* f(m) */
        if (is_prime(val))
        {
            ensure_capacity(cache_sz + 1, &cache, &cache_cap);
            cache[cache_sz++] = (seq_t)cand;    /* store m, not the value f(m) */
        }
    }
    return cache[n - 1]; /* a(1) is at index 0 */
}
