/*
 * A217698  Primes of the form ((2^p -1)*(3^p -2))-2, where p is prime.
 * https://oeis.org/A217698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of the form ((2^p-1)*(3^p-2))-2 where p is prime
 *            and the value is prime. The sequence stores the i‑th such prime
 *            for i ≥ 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big, next_prime, pow2, pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of computed terms and generates new ones by
   iterating over primes p using next_prime. For each p, it evaluates the
   expression using pow2, pow3, seq_mul and checks primality with is_prime_big. */

#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of offset; undefined but return 0 */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cached_len = 0;
    static size_t cached_cap = 0;

    /* continue generating until we have at least n terms */
    static uint64_t current_prime = 2;     /* next prime to test */

    while (cached_len < n) {
        /* Ensure cache has space for at least one more term */
        ensure_capacity(cached_len + 1, &cached_cap, (void **)&cache, sizeof(seq_t));

        seq_t p = (seq_t)current_prime;

        /* Compute 2^p and 3^p */
        seq_t two_pow = pow2(current_prime);   /* 2^p  */
        seq_t three_pow = pow3(current_prime); /* 3^p  */

        /* Compute ((2^p - 1) * (3^p - 2)) - 2 */
        seq_t term = seq_mul(two_pow - 1, three_pow - 2) - 2;

        /* If the term is prime, store it */
        if (is_prime_big(term)) {
            cache[cached_len++] = term;
        }

        /* Move to the next prime p */
        current_prime = (uint64_t)next_prime(current_prime);
    }

    return cache[n - 1];
}
