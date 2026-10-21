/*
 * A200219  Number of solutions of the equation x^n + (x+1)^n == (x+2)^n (mod
 *          n) for x = 0..n-1.
 * https://oeis.org/A200219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of x in [0, n-1] such that * x^n + (x+1)^n ≡
 *            (x+2)^n (mod n)
 * Range:     u8 n <= 412, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* ---- memoisation ---- */
    static seq_t *cache = NULL;
    static bool *computed = NULL;
    static size_t cap = 0;

    if (n >= cap) {
        size_t new_cap = n + 1;                    /* protect index n */
        seq_t *new_cache   = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        bool   *new_comp   = (bool *)realloc((void *)computed, new_cap * sizeof(bool));
        if (!new_cache || !new_comp) return 0;     /* allocation failure */

        /* initialise new entries */
        for (size_t i = cap; i < new_cap; ++i) {
            new_cache[i] = 0;   /* value will be set when computed */
            new_comp[i]  = false;
        }

        cache    = new_cache;
        computed = new_comp;
        cap      = new_cap;
    }

    if (computed[n])
        return cache[n];

    /* ---- compute value ---- */
    seq_t result = 0;

    if (is_prime(n)) {
        /* prime n ⇒ exactly one solution */
        result = 1;
    } else {
        seq_t N = (seq_t)n;        /* modulus */
        for (idx_t x = 0; x < n; ++x) {
            seq_t a1 = mod_pow((seq_t)x,     n, N);
            seq_t a2 = mod_pow((seq_t)(x+1), n, N);
            seq_t a3 = mod_pow((seq_t)(x+2), n, N);
            if (((a1 + a2) % N) == a3)
                ++result;
        }
    }

    cache[n] = result;
    computed[n] = true;
    return result;
}
