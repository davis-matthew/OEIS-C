/*
 * A171527  Numerator of (n-th composite/n).
 * https://oeis.org/A171527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = numerator of A002808(n)/n
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */
#include <math.h>     /* sqrt */
#include "../utils/gcd.h"

/* Cache of n-th composite numbers */
static seq_t *composites = NULL;
static size_t comp_cap   = 0;
static size_t comp_cnt   = 0;

/* Cache of primes found during composite generation */
static uint64_t *primes   = NULL;
static size_t primes_cap   = 0;
static size_t primes_cnt   = 0;

/* Ensure that composites[0 .. n-1] are filled (n ≥ 1). */
static void
ensure_composites(idx_t n)
{
    if (n <= comp_cnt)
        return; /* already computed */

    /* The first composite is 4 (the first integer >1 that is not prime). */
    static uint64_t last_candidate = 3; /* next number to test */

    /* Initialize primes list with 2 and 3. */
    if (primes_cnt == 0) {
        primes_cap = 4;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes[1] = 3;
        primes_cnt = 2;
    }

    /* Grow composites array if necessary. */
    if (comp_cap == 0) {
        comp_cap = 4;
        composites = (seq_t *)realloc(composites, comp_cap * sizeof(seq_t));
    }

    while (comp_cnt < (size_t)n) {
        last_candidate++;                     /* next candidate */

        bool is_prime = true;

        /* Trial division using known primes up to sqrt(candidate). */
        uint64_t limit = (uint64_t)sqrt((double)last_candidate);
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > limit)
                break;
            if (last_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* New prime found: store it for future tests. */
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 4;
                primes = (uint64_t *)realloc(primes,
                                            primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = last_candidate;
        } else {
            /* Composite number: store it. */
            if (comp_cnt == comp_cap) {
                comp_cap = comp_cap ? comp_cap * 2 : 4;
                composites = (seq_t *)realloc(composites,
                                              comp_cap * sizeof(seq_t));
            }
            composites[comp_cnt++] = (seq_t)last_candidate;
        }
    }
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of range, not used by tests */

    ensure_composites(n);

    seq_t composite = composites[n - 1];
    seq_t g = gcd(composite, (seq_t)n);   /* gcd for seq_t types */
    return composite / g;
}
