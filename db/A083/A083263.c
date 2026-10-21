/*
 * A083263  Numbers k such that the difference of the largest and smallest
 *          prime factors of k divides k.
 * https://oeis.org/A083263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer k such that *
 *            (largest_prime_factor(k) – smallest_prime_factor(k)) divides k.
 *            * The implementation searches for valid k’s, caching them so
 *            that * any order of calls with n ≥ 1 is answered efficiently
 * Range:     u8 n <= 25, u16 n <= 977, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t cache[20000];     /* Cached sequence terms                */
static size_t cache_len = 0;   /* Number of terms currently cached     */
static uint64_t next_k = 2;    /* Next integer to test for membership */

/* Return true iff k satisfies the defining property. */
static bool is_valid_k(uint64_t k)
{
    if (k <= 1) return false;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(k, pr, ex, &cnt);

    if (cnt == 0) return false;   /* Should not happen for k > 1 */

    uint64_t lpf = UINT64_MAX;
    uint64_t gpf = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] < lpf) lpf = pr[i];
        if (pr[i] > gpf) gpf = pr[i];
    }
    if (lpf == gpf) return false; /* Only one distinct prime factor */

    uint64_t diff = gpf - lpf;
    return (k % diff) == 0;
}

/* Ensure that at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    if ((idx_t)cache_len >= n) return;

    while ((idx_t)cache_len < n) {
        if (is_valid_k(next_k)) {
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* Sequence generator required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1; guard for safety. */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
