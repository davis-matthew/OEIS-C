/*
 * A037039  Least k such that 4*n*k+1 is a prime.
 * https://oeis.org/A037039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that 4*n*k+1 is prime
 * Range:     u8 n <= 20, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test if a seq_t number is prime, using trial division. */
static bool is_prime_seq(seq_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (seq_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* OEIS offset is 1; for n<1 we return 0 (undefined). */
    if (n < 1) return 0;
    for (idx_t k = 1; ; ++k) {
        seq_t val = (seq_t)4 * (seq_t)n * (seq_t)k + 1;
        if (is_prime_seq(val)) {
            return (seq_t)k;
        }
    }
}
