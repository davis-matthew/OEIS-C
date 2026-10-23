/*
 * A121402  Prime values of integers written in factorial base, interpreted as
 *          in base 10.
 * https://oeis.org/A121402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth prime that equals the decimal interpretation of
 *            the factorial–base representation of some integer m (starting
 *            from m = 1)
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n-th term of sequence A121402. */
seq_t a(idx_t n)
{
    /* Keep a static cache of all primes found so far. */
    static seq_t *cache = NULL;     /* pointer to cached primes       */
    static size_t cache_len = 0;    /* number of primes currently stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache      */
    static idx_t last_m = 1;        /* last integer that was processed  */

    if (n == 0)
        return 0;   /* undefined for 0, but avoid UB */

    /* Grow the cache until it contains at least n primes. */
    while (cache_len < (size_t)n)
    {
        /* Next integer to examine. */
        ++last_m;
        idx_t t = last_m;
        int digits[64];        /* factorial–base digits (least significant first) */
        size_t len = 0;
        int i = 1;             /* position index (factorial base = i+1) */

        /* Compute the factorial base representation of last_m. */
        while (t > 0 && i < 64)
        {
            digits[len] = (int)(t % (idx_t)(i + 1));
            t /= (idx_t)(i + 1);
            ++len;
            ++i;
        }

        /* Interpret those digits as a base‑10 integer. */
        seq_t val = 0;
        for (int j = (int)len - 1; j >= 0; --j)
            val = val * 10 + (seq_t)digits[j];

        /* If the value is prime, add it to the cache. */
        if (is_prime_big(val))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                {
                    /* Allocation failure – abort the program (no clean‑up). */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)n - 1];
}
