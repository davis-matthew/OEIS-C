/*
 * A078332  First prime such that p^2 == -1 (mod n), or 0 if no such prime
 *          exists.
 * https://oeis.org/A078332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first prime p such that p² ≡ –1 (mod n), or 0 if no such
 *            prime exists. (Index n is 1‑based.)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Return the first prime p with p² ≡ –1 (mod n). */
seq_t a(idx_t n)
{
    /* Handle the special case n == 1 – any prime works; choose 2. */
    if (n == (idx_t)1) {
        return (seq_t)2;
    }

    /* Choose a search bound sufficient for n up to about 10000. */
    idx_t limit = (n <= 5 ? (idx_t)20 : n * (idx_t)10 + (idx_t)100);

    uint64_t *primes = NULL;
    size_t pcnt   = 0;

    /* Generate all primes up to the bound. */
    build_primes_upto(limit, &primes, &pcnt);

    /* Target value is (-1 mod n) == n-1. */
    uint64_t target = (uint64_t)n - 1ULL;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];

        /* Compute p² mod n safely using 128‑bit intermediate. */
        uint64_t mod_sq =
            (uint64_t)((__uint128_t)p * (unsigned __int128)p % (unsigned __int128)n);

        if (mod_sq == target) {
            free(primes);
            return (seq_t)p;
        }
    }

    /* No prime within the searched range satisfies the congruence. */
    free(primes);
    return (seq_t)0;
}
