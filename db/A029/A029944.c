/*
 * A029944  a(n)^2 contains n-th prime as a substring.
 * https://oeis.org/A029944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 8152, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute force search for smallest a such that a^2
   contains the nth prime as a decimal substring.
   a(1) is the first term, etc. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* allocated size (1‑based) */

    /* allocate or grow cache if needed */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;   /* inclusive for index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;                       /* allocation failed */

        /* initialise new entries to 0 (not computed) */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;

        cache = new_cache;
        cache_size = new_size;
    }

    /* return cached value if already computed */
    if (cache[n])
        return cache[n];

    /* get the nth prime as seq_t */
    uint32_t prime_u32 = get_prime(n);
    seq_t    prime     = (seq_t)prime_u32;

    /* extract decimal digits of the prime */
    uint8_t  prime_buf[64];
    size_t   prime_len = 0;
    get_digits(prime, prime_buf, &prime_len);

    /* search for smallest a */
    seq_t a_val = 1;
    while (1)
    {
        /* square */
        seq_t sq = seq_mul(a_val, a_val);

        /* digits of square */
        uint8_t  sq_buf[256];
        size_t   sq_len = 0;
        get_digits(sq, sq_buf, &sq_len);

        if (sq_len >= prime_len)
        {
            /* check for prime substring */
            bool has = false;
            for (size_t i = 0; i <= sq_len - prime_len; ++i)
            {
                size_t j;
                for (j = 0; j < prime_len; ++j)
                {
                    if (sq_buf[i + j] != prime_buf[j])
                        break;
                }
                if (j == prime_len)
                {
                    has = true;
                    break;
                }
            }
            if (has)
            {
                cache[n] = a_val;
                return a_val;
            }
        }
        ++a_val;
    }
}
