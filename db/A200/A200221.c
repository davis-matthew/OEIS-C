/*
 * A200221  Ordered factorizations of n with 3 parts.
 * https://oeis.org/A200221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d≥2, n/d≥2} (τ(n/d) – 2)
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where τ(m) is the number of positive divisors of m. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of divisor counts τ(m) for 1 ≤ m ≤ max_cached */
static uint32_t *tau_cache = NULL;
static idx_t tau_cap = 0;   /* number of elements allocated (size) */

/* Ensure τ[1..limit] is available; recompute if necessary. */
static void ensure_tau(idx_t limit) {
    if (tau_cap > limit) return;

    uint32_t *buf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!buf) exit(EXIT_FAILURE);

    /* initialise to 0 */
    for (idx_t i = 0; i <= limit; ++i) buf[i] = 0;

    /* divisor sieve */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            buf[j] += 1;
        }
    }

    free(tau_cache);
    tau_cache = buf;
    tau_cap   = limit + 1;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1)=0; overtly handled */

    ensure_tau(n);

    seq_t result = 0;

    /* iterate over divisors i of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        idx_t d1 = i;          /* divisor */
        idx_t d2 = n / i;      /* complementary divisor */

        /* contribution for a = d1 */
        if (d1 >= 2 && d2 >= 2) {
            result += (seq_t)tau_cache[d2] - 2;
            if (d2 != d1) {
                /* contribution for a = d2 */
                result += (seq_t)tau_cache[d1] - 2;
            }
        }
    }

    return result;
}
