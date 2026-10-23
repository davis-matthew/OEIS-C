/*
 * A167833  Three-times-isolated primes: primes p such that neither p+-2, p+-4
 *          nor p+-6 is prime.
 * https://oeis.org/A167833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1274, u32 n <= 1619, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function returns the nth three‑times‑isolated prime.
   A prime p is three‑times‑isolated if none of p±2, p±4, p±6
   are prime.  The implementation searches primes incrementally
   and memoises the results so that subsequent calls do not
   recompute earlier terms.  The routine is efficient enough
   for n up to about 10 000. 
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of cached terms */
    static size_t cap   = 0;   /* capacity of cache */
    /* next candidate to test (2, then odd numbers) */
    static seq_t next = 2;

    while (len < n)
    {
        seq_t cand = next;
        bool ok = false;

        if (cand == (seq_t)2) {
            /* 2 is isolated by definition */
            ok = true;
        } else if (is_prime_big(cand)) {
            /* check isolation */
            if (!is_prime_big(cand + (seq_t)2) &&
                cand > (seq_t)2   && !is_prime_big(cand - (seq_t)2) &&
                !is_prime_big(cand + (seq_t)4)             &&
                cand > (seq_t)4   && !is_prime_big(cand - (seq_t)4) &&
                !is_prime_big(cand + (seq_t)6)             &&
                cand > (seq_t)6   && !is_prime_big(cand - (seq_t)6))
            {
                ok = true;
            }
        }

        if (ok)
        {
            /* grow cache if needed */
            if (len + 1 > cap)
            {
                size_t new_cap = cap ? (cap << 1) : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = cand;
        }

        /* advance to next candidate: start with 3, then odd numbers */
        if (next == (seq_t)2)
            next = (seq_t)3;
        else
            next += (seq_t)2;
    }

    return cache[n - (idx_t)1];
}
