/*
 * A215361  Primes congruent to {0, 2, 3, 4} mod 19.
 * https://oeis.org/A215361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 961 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: collect primes p such that p ≡ 0,2,3,4 (mod 19).
   The sequence a(n) lists the n‑th prime in that residue class. */
/* Helper for primality testing.  The implementation is linked separately. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache of primes satisfying the residue condition.  */
    static seq_t *cache = NULL;   /* array of primes kept as seq_t */
    static size_t  cap   = 0;     /* allocated capacity of cache   */
    static size_t  sz    = 0;     /* number of primes stored       */
    static uint64_t last = 2;     /* next integer to test          */

    /* Undefined behaviour for n == 0 – return 0 as a safeguard. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure cache contains at least n primes. */
    if (n > sz) {
        /* Initialise cache capacity if this is the first call. */
        if (cap == 0) {
            cap = 128;
            cache = mem_new(seq_t, cap);
        }

        /* Generate primes and store those that satisfy the required residues. */
        uint64_t cand = last;
        while (sz < n) {
            if (is_prime_seq(cand)) {
                uint64_t r = cand % 19;
                if (r == 0 || r == 2 || r == 3 || r == 4) {
                    if (sz == cap) {
                        cap <<= 1;  /* double the capacity */
                        cache = mem_resize(seq_t, cache, cap);
                    }
                    cache[sz++] = (seq_t)cand;
                }
            }
            ++cand;
        }
        /* Update the next starting point for future searches. */
        last = cand;
    }

    return cache[n - 1];
}
