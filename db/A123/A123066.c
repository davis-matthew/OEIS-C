/*
 * A123066  (Number of numbers <= n with an odd number of distinct prime
 *          factors) - (number of numbers <= n with an even number of distinct
 *          prime factors).
 * https://oeis.org/A123066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, look, sign
 * Formula:   not stated
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a sieve to compute
   a(n) = Σ_{k=2..n} (-1)^{ω(k)+1}
   where ω(k) is the number of distinct prime factors of k.
   The function keeps a static prefix sum table so that
   subsequent calls are O(1).                                                    */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of prefix sums */
    static seq_t *prefix = NULL;   /* prefix[i] = a(i) */
    static size_t prefix_cap = 0;  /* currently stored maximum index */

    /* if cache insufficient, rebuild up to n */
    if ((size_t)n > prefix_cap) {
        /* free old cache */
        if (prefix) {
            free(prefix);
            prefix = NULL;
        }

        /* allocate new cache */
        size_t cap = (size_t)n + 1;          /* include index 0 */
        prefix = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!prefix) {                       /* malloc failed */
            return 0;
        }

        /* sieve arrays */
        bool *isComp   = (bool *)calloc(cap, sizeof(bool));
        uint8_t *parity = (uint8_t *)calloc(cap, sizeof(uint8_t));
        if (!isComp || !parity) {
            free(prefix);
            free(isComp);
            free(parity);
            return 0;
        }

        /* sieve of Eratosthenes: build primes and toggle parity */
        for (idx_t i = 2; i <= n; ++i) {
            if (!isComp[(size_t)i]) { /* i is prime */
                /* toggle parity for all multiples of i */
                for (idx_t j = i; j <= n; j += i) {
                    parity[(size_t)j] ^= 1U;
                }
                /* mark composites */
                if ((idx_t)(i + i) <= n) {
                    for (idx_t j = i + i; j <= n; j += i) {
                        isComp[(size_t)j] = true;
                    }
                }
            }
        }

        /* compute prefix sums a(1..n) */
        seq_t sum = 0;            /* sum for k=2..current */
        prefix[0] = 0;            /* unused, but safe */
        if (n >= 1) prefix[1] = 0;/* a(1)=0 */
        for (idx_t k = 2; k <= n; ++k) {
            sum += (parity[(size_t)k] ? 1 : -1);
            prefix[(size_t)k] = sum;
        }

        /* clean up */
        free(isComp);
        free(parity);

        /* update cache capacity */
        prefix_cap = (size_t)n;
    }

    /* cache valid, return value */
    return prefix[(size_t)n];
}
