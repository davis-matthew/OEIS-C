/*
 * A139827  Primes of the form 2x^2 + 2xy + 17y^2.
 * https://oeis.org/A139827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 830, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A139827: primes of the form 2*x^2 + 2*x*y + 17*y^2
   Computation method: sieve primes up to a fixed limit (2 000 000);
   keep those with residues {2,17,29,41,65,101} modulo 132.
   The sequence is returned in zero‑based cache order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static inline bool matches_residue(uint32_t p)
{
    /* The required residues modulo 132.  2 is included explicitly. */
    static const int residues[6] = {2, 17, 29, 41, 65, 101};
    uint32_t r = p % 132;
    for (int i = 0; i < 6; ++i)
        if (r == (uint32_t)residues[i]) return true;
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of representable primes.  Filled once on first call. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static bool   init      = false;

    if (!init) {
        /* Upper bound for sieve, enough for n <= 10 000. */
        const size_t LIMIT = 2000000;

        /* Sieve of Eratosthenes */
        bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
        if (!is_prime) return 0;                     /* allocation failure */

        for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= LIMIT; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;

        /* Count primes to determine storage size. */
        size_t prime_cnt = 0;
        for (size_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i]) ++prime_cnt;

        /* Allocate cache large enough to hold all primes (upper bound). */
        cache = (seq_t*)malloc(prime_cnt * sizeof(seq_t));
        if (!cache) { free(is_prime); return 0; }

        /* Collect representable primes. */
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (!is_prime[i]) continue;
            uint32_t p = (uint32_t)i;
            if (matches_residue(p))
                cache[cache_len++] = (seq_t)p;
        }

        free(is_prime);
        init = true;
    }

    if (n == 0 || (size_t)n > cache_len) return 0;
    return cache[n - 1];
}
