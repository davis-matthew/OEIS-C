/*
 * A156657  Numbers that are not safe primes.
 * https://oeis.org/A156657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth natural number that is NOT a safe prime. */

#include <stdbool.h>
#include <stdint.h>

/* Simple primality test for 64‑bit unsigned integers. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t cnt = 0;
    uint64_t val = 0;

    while (cnt < target) {
        bool is_safe = false;
        if (val >= 5 && (val & 1)) {
            if (is_prime_uint64(val) &&
                is_prime_uint64((val - 1) / 2)) {
                is_safe = true;
            }
        }
        if (!is_safe) {
            ++cnt;
            if (cnt == target)
                return (seq_t)val;
        }
        ++val;
    }
    /* In practice this point is unreachable. */
    return 0;
}
