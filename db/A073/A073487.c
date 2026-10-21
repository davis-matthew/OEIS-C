/*
 * A073487  Squarefree numbers having exactly one prime gap.
 * https://oeis.org/A073487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th square‑free integer that has exactly one prime
 *            gap between consecutive primes in its factorisation
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_squarefree, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"    /* factor_u64 */
#include "../utils/is_squarefree.h"/* is_squarefree */
#include "../utils/next_prime.h"   /* next_prime */

/* Compute the n-th entry of OEIS sequence A073487. */
seq_t a(idx_t n)
{
    uint64_t val = 2;            /* current integer to test */
    idx_t   count = 0;           /* how many terms found yet */

    while (count < n) {
        /* Check square‑free */
        if (is_squarefree((seq_t)val)) {
            /* Factorise val */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t   cnt;

            factor_u64(val, primes, exps, &cnt);

            if (cnt >= 2) {
                uint32_t gaps = 0;
                for (size_t i = 0; i + 1 < cnt; ++i) {
                    uint64_t p = primes[i];
                    uint64_t q = primes[i + 1];
                    if (next_prime(p) != q) { /* primes not consecutive */
                        ++gaps;
                        if (gaps > 1) break;
                    }
                }
                if (gaps == 1) {
                    ++count;
                    if (count == n) {
                        return (seq_t)val;
                    }
                }
            }
        }
        ++val;
    }
    /* The loop always returns before reaching this point. */
    return 0;
}
