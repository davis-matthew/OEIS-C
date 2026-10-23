/*
 * A175557  Prime preperiodic part of the decimal expansion of 1/k as k runs
 *          through A065502.
 * https://oeis.org/A175557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A175557.
   A(n) is the nth prime that occurs as the integer preperiodic part
   of the decimal expansion of 1/k for k that are multiples of 2 or 5.
   For each such k let
        L = max(v2(k), v5(k))          // maximum exponent of 2 or 5 in k
   then the preperiodic part is
        p = floor(10^L / k)
   If p is prime it is recorded as the next element of the sequence. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* static cache of the primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next k to test (smallest multiple of 2 or 5 not yet examined) */
    static idx_t next_k = 2;

    if (n == 0) return 0;          /* undefined for n <= 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* skip k that are not multiples of 2 or 5 */
        while ((next_k % 2ULL != 0) && (next_k % 5ULL != 0))
            ++next_k;

        idx_t k = next_k;

        /* count powers of 2 and 5 in k */
        idx_t tmp = k;
        idx_t exp2 = 0;
        idx_t exp5 = 0;
        while ((tmp & 1ULL) == 0) { ++exp2; tmp >>= 1; }
        while (tmp % 5ULL == 0)  { ++exp5; tmp /= 5ULL; }
        idx_t m = tmp;  /* the remaining factor (gcd(m,10)==1) */

        /* L is the maximum of the 2‑ and 5‑exponents */
        idx_t L = (exp2 > exp5 ? exp2 : exp5);

        /* compute the preperiodic part p = floor(10^L / k) */
        seq_t num = pow10((idx_t)L);     /* 10^L as seq_t */
        seq_t p   = num / (seq_t)k;      /* integer division */

        /* record p if it is prime */
        if (is_prime_big(p))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) /* out of memory: abort with 0 */
                    return 0;
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }

        ++next_k;   /* proceed to next candidate */
    }

    return cache[n - 1];
}
