/*
 * A088633  P-n where P = smallest prime > 2n.
 * https://oeis.org/A088633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(n) – n, where P(n) is the smallest prime strictly
 *            greater than 2n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Small deterministic primality test for 64‑bit values */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    if (n % 3ULL == 0ULL) return n == 3ULL;
    uint64_t i = 5ULL;
    uint64_t w = 2ULL;  // alternating step 2,4 (6‑k rule)
    for (;;) {
        if (i * i > n) break;
        if (n % i == 0ULL) return false;
        i += w;
        w = 6ULL - w;
    }
    return true;
}

/* Find the smallest prime strictly greater than x */
static uint64_t next_prime_gt_uint64(uint64_t x) {
    if (x < 2ULL) return 2ULL;
    uint64_t cand = x + 1ULL;
    if (cand % 2ULL == 0ULL) {
        if (cand == 2ULL) return 2ULL;
        cand++;
    }
    for (;;) {
        if (is_prime_uint64(cand)) return cand;
        cand += 2ULL;
    }
}

/* Singleton sequence function */
seq_t a(idx_t n) {
    uint64_t two_n = (uint64_t)n * 2ULL;
    uint64_t p = next_prime_gt_uint64(two_n);
    return (seq_t)p - (seq_t)n;
}
