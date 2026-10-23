/*
 * A037450  Numbers which are one less than a perfect square that cannot
 *          otherwise be written as a power.
 * https://oeis.org/A037450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007916(n)^2 - 1
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007916(n) is the n-th positive integer that is not a perfect power */

#include <stdbool.h>

/* Return true iff m is a perfect power (m = b^e with e >= 2). */
static bool is_perfect_power(idx_t m) {
    if (m <= 1) return true;          /* 1 is a perfect power (1^e) */

    seq_t target = (seq_t)m;
    for (idx_t base = 2; base <= m / base; ++base) {
        seq_t power = (seq_t)base * (seq_t)base;   /* base^2 */
        while (power <= target) {
            if (power == target) return true;
            power *= (seq_t)base;                 /* base^e -> base^(e+1) */
        }
    }
    return false;
}

seq_t a(idx_t n) {
    /* Find the n-th non‑perfect‑power integer k and return k^2-1. */
    idx_t k = 2;
    idx_t count = 0;
    while (true) {
        if (!is_perfect_power(k)) {
            ++count;
            if (count == n) break;
        }
        ++k;
    }
    seq_t kk = (seq_t)k;
    return kk * kk - 1;
}
