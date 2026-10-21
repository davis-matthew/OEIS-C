/*
 * A095697  Primes of the form x^2 + y^2 + z, where x, y and z are three
 *          successive numbers.
 * https://oeis.org/A095697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A095697 – Primes of the form 2*k^2 + 3*k + 3 (k = 0,1,2,…).
 *  a(n) is the n‑th prime among the values f(k)=2*k^2+3*k+3
 *  (offset = 1: a(1)=3, a(2)=17, …).
 *  Implementation: iterate k from 0, test f(k) for primality (using the
 *  provided isPrime utility), store found primes in a static cache.
 *  The function works for any seq_t defined by the build; values that
 *  fit in 64 bits are passed to isPrime after casting.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* Cache of primes satisfying f(k). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of stored primes */
    static idx_t cache_cap  = 0; /* allocated capacity */
    static idx_t next_k = 0;     /* next k to evaluate */

    if (n == 0) /* offset is 1, undefined for n<1 */
        return 0;

    /* Extend cache until it contains at least n primes. */
    while (cache_size < n)
    {
        seq_t k   = (seq_t)next_k;
        seq_t val = (seq_t)2 * k * k + (seq_t)3 * k + (seq_t)3;

        /* Primality test: cast to idx_t (fits for all generated values). */
        bool is_prime = isPrime((idx_t)val);

        if (is_prime)
        {
            if (cache_size == cache_cap)
            {
                idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                if (!tmp)
                {
                    /* Allocation failed – return 0 for safety. */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }

        ++next_k;
    }

    return cache[n - 1];
}
