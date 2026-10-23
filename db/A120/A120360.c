/*
 * A120360  Even refactorable numbers k such that the number of odd divisors
 *          of k and the number of even divisors of k are both even divisors
 *          of k.
 * https://oeis.org/A120360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_of_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A120360 – Even refactorable numbers
 * a(n) = the n‑th even integer k such that
 *   k = 2^e * m  (m odd)
 *   r = τ(m)        (number of odd divisors of k)
 *   s = e · r       (number of even divisors of k)
 *   r is even and r divides k,
 *   s is even and s divides k,
 *   and k is refactorable: k % ((e+1)·r) == 0. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/tau_of_odd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t next_k = 2;      /* next even integer to test */

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    while (cache_size < n) {
        idx_t k = next_k;

        /* Extract exponent of 2 (e) and the odd part m. */
        uint64_t tmp = (uint64_t)k;
        uint64_t e = 0;
        while ((tmp & 1ULL) == 0) {
            e++;
            tmp >>= 1;
        }
        uint64_t m = tmp;

        seq_t r = tau_of_odd(m);

        /* r must be even. */
        if (((uint64_t)r) & 1ULL) {
            next_k += 2;
            continue;
        }

        /* r must divide k. */
        if (((seq_t)k) % r != 0) {
            next_k += 2;
            continue;
        }

        /* s = e * r must divide k. */
        seq_t prod = (seq_t)e * r;
        if (prod > (seq_t)k) { next_k += 2; continue; }
        if (((seq_t)k) % prod != 0) { next_k += 2; continue; }

        /* k must be refactorable: k divisible by τ(k) = (e+1)*r. */
        if (((seq_t)k) % ((seq_t)(e + 1) * r) != 0) {
            next_k += 2;
            continue;
        }

        /* All conditions satisfied – store the term. */
        if (cache_size == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = (seq_t)k;

        next_k += 2;
    }

    return cache[n - 1];
}
