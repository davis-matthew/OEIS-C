/*
 * A107168  Primes of the form 4x^2 + 15y^2.
 * https://oeis.org/A107168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that p = 4x² + 15y², equivalently p ≡ 19 or 31
 *            (mod 60)
 * Range:     u8 n <= 7, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9924 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static idx_t cap = 0;
    static idx_t count = 0;
    static uint64_t cand = 19;          /* first number ≡ 19 (mod 60) */

    if (n == 0) return 0;              /* behaviour below offset is irrelevant */

    while (count < n) {
        /* Search for the next prime with the desired residue. */
        while (1) {
            uint32_t r = cand % 60;
            if (r == 19u || r == 31u) {
                if (is_prime_u64(cand))
                    break;          /* we have the next term */
            }
            cand++;
        }

        /* Grow storage if necessary. */
        if (count >= cap) {
            idx_t new_cap = cap ? (cap << 1) : 16;
            seq_t *tmp = seq_realloc(primes, new_cap);
            if (!tmp) return 0;      /* allocation failure – unlikely */
            primes = tmp;
            cap = new_cap;
        }

        primes[count] = (seq_t)cand;   /* store the found prime */
        count++;
        cand++;                        /* next candidate to test */
    }

    return primes[n - 1];
}
