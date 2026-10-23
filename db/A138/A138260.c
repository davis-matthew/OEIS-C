/*
 * A138260  Smallest positive integer m such that n divides [7^m/m]
 *          (=A129797(m)).
 * https://oeis.org/A138260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 138, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest positive integer m such that n divides floor(7^m/m).
   Equivalent condition: 7^m mod (n*m) < m. */

#include <stdint.h>
#include <stdlib.h>

/* fast modular exponentiation using 64‑bit arithmetic (safe for intermediate
   products with __int128).  Returns base^exp modulo mod.  */
static inline uint64_t modpow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t p = base % mod;
    while (exp > 0)
    {
        if (exp & 1u)
            result = (unsigned __int128)result * p % mod;
        p = (unsigned __int128)p * p % mod;
        exp >>= 1u;
    }
    return result;
}

/* Memoised answers for a(n).  0 is used as an unused sentinel value. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* n >= 1 due to OEIS offset.  */
    if (n == 0)
        return 0;                 /* undefined, but keep harmless */

    /* Expand cache if needed.  */
    if (n >= cache_size)
    {
        size_t new_size = n + 1;     /* indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL)
            abort();                /* allocation failed */
        /* initialise the new part to 0 */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Return cached result if present. */
    if (cache[n] != 0)
        return cache[n];

    /* Search for the smallest m.  */
    for (idx_t m = 1;; ++m)
    {
        uint64_t mod = (uint64_t)n * (uint64_t)m;
        uint64_t rem = modpow_uint64(7, m, mod);
        if (rem < m)                 /* 7^m mod (n*m) < m  */
        {
            seq_t ans = (seq_t)m;
            cache[n] = ans;          /* store for future calls */
            return ans;
        }
    }
}
