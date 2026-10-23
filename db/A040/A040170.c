/*
 * A040170  Primes p such that x^5 = 6 has no solution mod p.
 * https://oeis.org/A040170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A040170 – primes p such that x^5 = 6 has no solution mod p.
   A prime p is in the sequence iff p ≡ 1 (mod 5) and 6 is NOT a 5th power
   modulo p, i.e. 6^((p−1)/5) ≢ 1 (mod p). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of known terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next integer to test; start from the smallest prime 2 */
    static uint64_t cand = 2;

    if (n == 0)   /* undefined for offset 0 */
        return 0;

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n)
    {
        /* skip even numbers after 2 */
        if (cand > 2 && (cand & 1U) == 0) {
            cand++;
            continue;
        }

        /* primality test */
        if (is_prime_small((seq_t)cand))
        {
            /* only p ≡ 1 (mod 5) can fail to be a fifth‑power residue */
            if ((cand % 5U) == 1U)
            {
                uint64_t exp = (cand - 1) / 5U;   /* (p-1)/5  */
                seq_t r = mod_pow((seq_t)6, exp, (seq_t)cand);

                if (r != (seq_t)1)
                {
                    /* grow cache if needed */
                    if (cache_len + 1 > cache_cap)
                    {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (tmp == NULL) /* out of memory */
                            abort();
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)cand;
                }
            }
        }
        cand++;
    }

    return cache[n - 1];
}
