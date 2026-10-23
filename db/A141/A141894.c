/*
 * A141894  Primes congruent to 13 mod 21.
 * https://oeis.org/A141894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 13 modulo 21
 * Range:     u8 n <= 5, u16 n <= 549, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iterating numbers of the form 21*k+13 and testing primality. */
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers. */
static inline bool is_prime_idx(idx_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1; return 0 as placeholder */

    idx_t count = 0;
    idx_t candidate = 13;   /* first prime ≡ 13 mod 21 */

    for (;;) {
        if (is_prime_idx(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 21;    /* step to next number ≡ 13 mod 21 */
    }
}
