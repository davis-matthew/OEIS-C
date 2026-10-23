/*
 * A033746  Expansion of Product_{d | 34} theta_3(q^d).
 * https://oeis.org/A033746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to n = k1^2 + 2*k2^2 +
 *            17*k3^2 + 34*k4^2
 * Range:     u8 n <= 339, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _mult_for_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_mult_for_k.h"

/* ------------------------------------------------------------------ */
/* Global cache of coefficients.  Cached up to seq_cache_max.        */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;   /* array of coefficients */
static idx_t seq_cache_max = 0;   /* highest index computed */

/* ------------------------------------------------------------------ */
/* Helper: return multiplicity for a given k.  1 if k==0 else 2.      */

/* ------------------------------------------------------------------ */
/* Recompute all coefficients from 0 up to N (inclusive).            */
/* ------------------------------------------------------------------ */
static void _compute_up_to(idx_t N)
{
    size_t len = (size_t)(N + 1);
    seq_t *new_cache = (seq_t *)malloc(len * sizeof(seq_t));
    for (size_t i = 0; i < len; ++i)
        new_cache[i] = 0;

    for (uint64_t k4 = 0;; ++k4) {
        uint64_t val4 = 34ULL * k4 * k4;
        if (val4 > N) break;
        seq_t mult4 = _mult_for_k(k4);

        for (uint64_t k3 = 0;; ++k3) {
            uint64_t val3 = 17ULL * k3 * k3;
            if (val3 + val4 > N) break;
            seq_t mult3 = _mult_for_k(k3);
            seq_t mult34 = mult3 * mult4;

            for (uint64_t k2 = 0;; ++k2) {
                uint64_t val2 = 2ULL * k2 * k2;
                if (val2 + val3 + val4 > N) break;
                seq_t mult2 = _mult_for_k(k2);
                seq_t mult234 = mult34 * mult2;

                for (uint64_t k1 = 0;; ++k1) {
                    uint64_t val1 = k1 * k1;
                    uint64_t sum = val1 + val2 + val3 + val4;
                    if (sum > N) break;
                    seq_t mult1 = _mult_for_k(k1);
                    new_cache[(size_t)sum] += mult1 * mult234;
                }
            }
        }
    }

    if (seq_cache)
        free(seq_cache);
    seq_cache = new_cache;
    seq_cache_max = N;
}

/* ------------------------------------------------------------------ */
/* Public function: a(idx_t n)                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (seq_cache == NULL)           /* first call – compute up to 0 */
        _compute_up_to(0);

    if (n > seq_cache_max)           /* need a larger range */
        _compute_up_to(n);

    return seq_cache[n];
}
