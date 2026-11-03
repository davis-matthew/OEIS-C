/*
 * A040149  Primes p such that x^4 = 21 has a solution mod p.
 * https://oeis.org/A040149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Find the nth prime p for which x^4 = 21 has a solution modulo p
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 973 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    /* static cache of the generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t candidate = 2;        /* next integer to test */

    /* generate until we have at least n terms */
    while (cache_len < n)
    {
        if (is_prime(candidate))
        {
            bool ok = false;

            /* if p divides 21 then x = 0 is a solution */
            if (21 % candidate == 0)
                ok = true;
            else
            {
                idx_t exp;
                if ((candidate - 1) % 4 == 0)          /* p ≡ 1 (mod 4) */
                    exp = (candidate - 1) / 4;
                else                                   /* p ≡ 3 (mod 4) */
                    exp = (candidate - 1) / 2;

                seq_t r = powmod((seq_t)21, exp, (seq_t)candidate);
                if (r == (seq_t)1)
                    ok = true;
            }

            if (ok)
            {
                /* grow cache if necessary */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return 0;    /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)candidate;
            }
        }

        /* advance to next candidate: skip even numbers after 2 */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }

    return cache[n - 1];
}
