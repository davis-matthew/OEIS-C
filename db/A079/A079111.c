/*
 * A079111  Numbers n such that 2*n+1 or n itself is prime.
 * https://oeis.org/A079111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A079111: a(n) is the n‑th integer k such that k is prime
   or 2*k+1 is prime.  The routine enumerates integers, tests
   each for primality by simple trial division, and returns the n‑th
   qualifying integer.  Offset 1. */

#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test suitable for n up to very
   large 64‑bit values; here it will be used only for numbers up to
   a few hundred thousand, so trial division is fast enough. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but harmless */
    idx_t count = 0;
    uint64_t k = 1;
    while (true) {
        if (is_prime_uint64(k) || is_prime_uint64(2 * k + 1)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
