/*
 * A053989  Smallest k such that nk-1 is prime.
 * https://oeis.org/A053989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k such that n*k-1 is prime
 * Range:     u8 n <= 42, u16 n <= 3310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for seq_t values.
   Works for any integer type that fits in seq_t and for values
   that fit in 64 bits, which is sufficient for n ≤ 10000 and
   the corresponding smallest primes encountered. */
static bool isPrime(seq_t v) {
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (seq_t i = 3; i <= v / i; i += 2) {
        if (v % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1, but guard against n<1 */
    for (idx_t k = 1; ; ++k) {
        seq_t val = (seq_t)n * (seq_t)k - 1;
        if (isPrime(val)) return (seq_t)k;
    }
}
