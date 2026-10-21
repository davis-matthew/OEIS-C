/*
 * A056563  Number of primes which are the difference between two triangular
 *          numbers, where the larger is the n-th triangular number.
 * https://oeis.org/A056563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n>=2 && prime(n) ? 1 : 0) + (n>=3 && prime(2*n-1) ? 1 :
 *            0)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_idx(idx_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    idx_t i = 5;
    while (i <= x / i) {
        if (x % i == 0) return false;
        if (x % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n >= 2 && is_prime_idx(n))
        ++count;
    if (n >= 3 && is_prime_idx(2 * n - 1))
        ++count;
    return count;
}
