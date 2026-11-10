/*
 * A084409  Main diagonal of triangle A084408.
 * https://oeis.org/A084409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(A034387(n)) for n prime; a(n) = A002808(n*(n+1)/2 -
 *            A034387(n) - 1) for n composite; a(1) = 1. A034387(n) = sum of
 *            primes <= n
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool A084409_is_prime(seq_t num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    if (num == 3) return true;
    if (num % 3 == 0) return false;
    for (seq_t i = 5; i <= num / i; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

static seq_t A084409_composite(seq_t target) {
    seq_t count = 0;
    seq_t cand = 4;
    while (true) {
        if (!A084409_is_prime(cand)) {
            count++;
            if (count == target) return cand;
        }
        cand++;
    }
}

static seq_t A084409_nth_prime(seq_t target) {
    seq_t count = 0;
    seq_t cand = 2;
    while (true) {
        if (A084409_is_prime(cand)) {
            count++;
            if (count == target) return cand;
        }
        cand++;
    }
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if (n <= 1) return 1;
    seq_t s = 0;
    for (seq_t k = 2; k <= N; k++) {
        if (A084409_is_prime(k)) s += k;
    }
    if (A084409_is_prime(N)) {
        return A084409_nth_prime(s);
    }
    seq_t idx = N * (N + 1) / 2 - s - 1;
    return A084409_composite(idx);
}
