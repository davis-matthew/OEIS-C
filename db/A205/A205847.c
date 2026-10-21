/*
 * A205847  Numbers k for which 4 divides s(k)-s(j) for some j<k; each k
 *          occurs once for each such j; s(k) denotes the (k+1)-st Fibonacci
 *          number.
 * https://oeis.org/A205847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205847: For each integer k ≥ 1, output the value k
   as many times as there are earlier indices j < k with
   s(k) ≡ s(j) (mod 4), where s(i) = F_{i+1} (the (i+1)-st
   Fibonacci number).  The implementation uses memoization
   and the 6‑periodic Fibonacci residue modulo 4. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range requests (the sequence is 1‑indexed). */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of computed sequence entries. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Count of earlier indices that had each residue of F_n mod 4. */
    static size_t count[4] = {0};

    /* Next k (starting from 1) that has not yet been processed. */
    static idx_t next_k = 1;

    while (cache_len < n)
    {
        idx_t k = next_k;                          /* current k */
        unsigned int mod_idx = (unsigned int)((k + 1) % 6); /* index in period 0‑5 */

        /* Residue of F_{k+1} modulo 4: period 0,1,1,2,3,1 */
        unsigned int f_mod;
        switch (mod_idx)
        {
            case 0: f_mod = 0; break;
            case 1: f_mod = 1; break;
            case 2: f_mod = 1; break;
            case 3: f_mod = 2; break;
            case 4: f_mod = 3; break;
            case 5: f_mod = 1; break;
            default: f_mod = 0; break; /* unreachable */
        }

        size_t mult = count[f_mod];          /* how many times to output k */

        /* Append k mult times to the cache. */
        if (mult > 0)
        {
            size_t required = cache_len + mult;
            if (required > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : required;
                if (new_cap < required)
                    new_cap = required;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                 /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            for (size_t i = 0; i < mult; ++i)
                cache[cache_len++] = (seq_t)k;
        }

        /* Record this k for future calls. */
        count[f_mod]++;

        ++next_k;
    }

    return cache[n - 1];
}
