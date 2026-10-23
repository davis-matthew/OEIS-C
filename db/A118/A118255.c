/*
 * A118255  a(1)=1, then a(n)=2*a(n-1) if n is prime, a(n)=2*a(n-1)+1 if n not
 *          prime.
 * https://oeis.org/A118255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = 2*a(n‑1) if n is prime a(n) = 2*a(n‑1)+1
 *            otherwise (indices start at 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* undefined indices return 0 */
    if (n < (idx_t)1)
        return 0;

    /* ----- static memoisation state ----- */
    static seq_t *cache      = NULL;   /* cache[1..current_max] holds a(i)   */
    static size_t cache_cap  = 0;       /* allocated size of cache array      */
    static size_t current_max = 0;      /* highest n already computed         */

    static bool *prime_flag  = NULL;   /* prime_flag[i] == true → i is prime */
    static size_t prime_cap  = 0;       /* size of prime_flag array           */

    /* ----- grow cache if necessary ----- */
    if (cache_cap < (size_t)(n + 1))
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ----- sieve up to n if necessary ----- */
    if (prime_cap < (size_t)(n + 1))
    {
        free(prime_flag);
        prime_flag = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
        if (!prime_flag)
            abort();

        /* initialise all as prime, then mark 0,1 as composite */
        for (idx_t i = 0; i <= n; ++i)
            prime_flag[i] = true;
        if (n >= 0) prime_flag[0] = false;
        if (n >= 1) prime_flag[1] = false;

        /* classical Eratosthenes sieve */
        for (idx_t p = 2; p <= n / p; ++p)
            if (prime_flag[p])
                for (idx_t j = p * p; j <= n; j += p)
                    prime_flag[j] = false;
        prime_cap = n + 1;
    }

    /* ----- initialise first term if needed ----- */
    if (current_max == 0)
    {
        /* cache array is large enough (at least 16) */
        cache[1] = 1;
        current_max = 1;
    }

    /* ----- compute terms up to n ----- */
    if (current_max < n)
    {
        for (idx_t i = current_max + 1; i <= n; ++i)
        {
            seq_t val = cache[i - 1];
            if (prime_flag[i])       /* prime */
                val <<= 1;           /* 2 * a(i-1) */
            else                      /* composite */
                val = (val << 1) + 1;/* 2 * a(i-1) + 1 */
            cache[i] = val;
        }
        current_max = n;
    }

    return cache[n];
}
