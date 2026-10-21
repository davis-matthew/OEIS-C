/*
 * A096500  Let f(n) = smallest prime > n; a(n) = f(n+1) - f(n).
 * https://oeis.org/A096500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = next_prime(n+1) – next_prime(n) (n ≥ 1)
 * Range:     u8 n <= 9967, u16 n <= 9967, u32 n <= 9967, u64 n <= 9967,
 *            u128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

static uint64_t next_prime_gt(uint64_t x) {
    if (x < 2ULL) return 2ULL;
    uint64_t p = x + 1ULL;
    if (p % 2ULL == 0ULL) p++;           /* make odd unless it is 2 */
    for (;;) {
        if (is_prime(p)) return p;
        p += 2ULL;
    }
}

seq_t a(idx_t n) {
    /* n is guaranteed to be ≥ 1 */
    uint64_t p1 = next_prime_gt((uint64_t)n);
    uint64_t p2 = next_prime_gt((uint64_t)(n + 1));
    return (seq_t)(p2 - p1);
}
