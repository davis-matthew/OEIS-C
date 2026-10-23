/*
 * A102656  Numbers k such that 11*k + 1 is prime.
 * https://oeis.org/A102656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that 11*k + 1 is prime (OEIS
 *            A102656)
 * Range:     u8 n <= 4, u16 n <= 892, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_seq, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/seq_realloc.h"

/* Return the nth term of the sequence, where n ≥ 1 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values of k */
    static idx_t cache_size = 0;
    static idx_t cached_count = 0;  /* number of cached terms */
    static idx_t next_k = 1;        /* next candidate k to test */

    if (n == 0) return 0;  /* safety guard – sequence is 1‑based */

    /* Generate terms until we have at least n of them */
    while (cached_count < n)
    {
        seq_t m = (seq_t)11 * (seq_t)next_k + 1;
        if (is_prime_seq(m))
        {
            /* Grow cache if necessary */
            if (cached_count == cache_size)
            {
                idx_t new_size = cache_size ? cache_size * 2 : 128;
                cache = seq_realloc(cache, new_size);
                cache_size = new_size;
            }
            cache[cached_count] = (seq_t)next_k;
            ++cached_count;
        }
        ++next_k;
    }
    return cache[n - 1];
}
