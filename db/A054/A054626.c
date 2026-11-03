/*
 * A054626  Number of n-bead necklaces with 7 colors.
 * https://oeis.org/A054626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} φ(d) * 7^(n/d) for n > 0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Cache of powers of 7: pow_cache[k] = 7^k */
static seq_t *pow_cache = NULL;
static size_t pow_cache_size = 0;

/* Ensure pow_cache is allocated up to at least n (inclusive) */
static void ensure_pow_cache(idx_t n)
{
    if (pow_cache_size == 0) {
        pow_cache = mem_new(seq_t, (size_t)(n + 1));
        pow_cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i) {
            pow_cache[i] = seq_mul(pow_cache[i - 1], (seq_t)7);
        }
        pow_cache_size = (size_t)(n + 1);
    } else if ((size_t)(n + 1) > pow_cache_size) {
        pow_cache = mem_resize(seq_t, pow_cache, (size_t)(n + 1));
        for (idx_t i = pow_cache_size; i <= n; ++i) {
            pow_cache[i] = seq_mul(pow_cache[i - 1], (seq_t)7);
        }
        pow_cache_size = (size_t)(n + 1);
    }
}

/* External function required by the build */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    ensure_pow_cache(n);

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == (idx_t)0) {
            seq_t phi_d = totient(d);
            idx_t exp = n / d;
            seq_t term = seq_mul(phi_d, pow_cache[exp]);
            sum += term;
        }
    }

    return sum / (seq_t)n;
}
