/*
 * A140775  Numbers k > 1 such that p + k/p is prime for every prime p that
 *          divides k.
 * https://oeis.org/A140775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 879, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A140775: iterate over even candidates, check square‑free
   and that p + k/p is prime for each prime divisor p of k. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;           /* memoised terms */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t cache_size = 0;          /* number of terms stored */
static uint64_t next_candidate = 2;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, but safe guard */

    /* Generate terms until we have at least n. */
    while (cache_size < n) {
        /* Only even numbers can satisfy the condition. */
        if ((next_candidate & 1U) == 0) {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;

            factor_u64(next_candidate, primes, exps, &cnt);

            /* Reject if not square‑free. */
            bool squarefree = true;
            for (size_t i = 0; i < cnt && squarefree; ++i) {
                if (exps[i] > 1) squarefree = false;
            }

            if (squarefree) {
                bool ok = true;
                seq_t k = (seq_t)next_candidate;
                for (size_t i = 0; i < cnt && ok; ++i) {
                    seq_t p = (seq_t)primes[i];
                    seq_t m = k / p;           /* integer division, exact */
                    seq_t s = p + m;           /* p + k/p */
                    if (!is_prime((idx_t)s))
                        ok = false;
                }
                if (ok) {
                    /* Resize storage if needed. */
                    if (cache_size >= cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 16;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_size++] = k;
                }
            }
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
