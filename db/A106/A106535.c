/*
 * A106535  Numbers k such that the smallest x > 1 for which Fibonacci(x) == 0
 *          mod k is x = k - 1.
 * https://oeis.org/A106535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p with p % 5 == 1 or 4 for which * the smallest
 *            x > 1 s.t. Fibonacci(x) == 0 (mod p) equals p-1
 * Range:     u8 n <= 11, u16 n <= 1159, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   fib_mod, is_prime, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/fib_mod.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* Computes the n-th term of OEIS sequence A106535. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of found terms          */
    static idx_t  cached_size = 0;       /* number of stored terms       */
    static idx_t  cached_cap  = 0;       /* capacity of the array        */
    static idx_t  next_prime = 2;        /* next candidate for primality */

    /* Ensure the cache exists */
    if (!cache) {
        cached_cap = 1;
        cache = seq_malloc(cached_cap);
    }

    /* Populate cache until we have at least n terms */
    while (cached_size < n) {
        /* Find next prime candidate */
        while (!is_prime((uint64_t)next_prime))
            ++next_prime;

        uint64_t p = (uint64_t)next_prime;            /* candidate prime    */
        ++next_prime;                                 /* prepare for next   */

        /* Skip 5 and primes with wrong residue mod 5 */
        if (p == 5 || (p % 5 != 1 && p % 5 != 4))
            continue;

        /* Check that no proper divisor d of p-1 satisfies F(d) ≡ 0 (mod p) */
        bool ok = true;
        idx_t m = (idx_t)p - 1;                       /* p-1                */
        for (idx_t d = 1; d * d <= m && ok; ++d) {
            if (m % d == 0) {
                idx_t d1 = d;
                idx_t d2 = m / d;

                if (d1 != m && fib_mod(d1, (idx_t)p) == 0)
                    ok = false;

                if (ok && d2 != d1 && d2 != m && fib_mod(d2, (idx_t)p) == 0)
                    ok = false;
            }
        }
        if (!ok)
            continue;           /* prime does not satisfy the condition */

        /* Store the prime in the cache */
        if (cached_size == cached_cap) {
            cached_cap <<= 1;
            if (cached_cap == 0) cached_cap = 1;
            seq_t *tmp = seq_realloc(cache, cached_cap);
            cache = tmp;
        }
        cache[cached_size++] = (seq_t)p;
    }

    return cache[n - 1];
}
