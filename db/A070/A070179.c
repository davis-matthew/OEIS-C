/*
 * A070179  Primes p such that x^2 = 2 has a solution mod p, but x^(2^2) = 2
 *          has no solution mod p.
 * https://oeis.org/A070179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p ≡ 1 (mod 8) such that 2^((p‑1)/4) ≡ –1
 *            (mod p)
 * Range:     u8 n <= 6, u16 n <= 809, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                              (OEIS A070179) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t n); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* Static state to allow calls in arbitrary order. */
    static uint64_t curr = 3;          /* next odd candidate (starts at 3) */
    static idx_t   count = 0;          /* how many special primes found so far */
    static uint64_t *primes = NULL;    /* array to store found primes */
    static size_t   cap    = 0;        /* allocated capacity of primes array */

    /* Generate primes until we have at least n of them. */
    while (count < n) {
        /* Find the next prime. */
        while (!is_prime(curr))
            curr += 2;                 /* skip even numbers */

        /* Only primes ≡ 1 (mod 8) can satisfy the condition. */
        if ((curr & 7U) == 1U) {
            uint64_t exp = (curr - 1) >> 2;          /* (p‑1)/4 */
            seq_t rem = mod_pow((seq_t)2, exp, (seq_t)curr);

            /* rem == p‑1 ⇒ rem ≡ –1 (mod p) */
            if (rem == (seq_t)(curr - 1)) {
                /* Ensure capacity for the new prime. */
                if (count == cap) {
                    size_t new_cap = (cap == 0) ? 4 : cap << 1;
                    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
                    if (!tmp)
                        abort();            /* memory allocation failed */
                    primes = tmp;
                    cap = new_cap;
                }
                primes[count++] = curr;
            }
        }

        curr += 2;    /* advance to next odd candidate */
    }

    return (seq_t)primes[n - 1];
}
