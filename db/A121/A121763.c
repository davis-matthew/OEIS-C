/*
 * A121763  Numbers k such that 6*k-1 is prime while 6*k+1 is composite.
 * https://oeis.org/A121763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Search: a(n) = the nth integer k (offset 1) such that 6*k-1 is prime
 *          while 6*k+1 is composite.
 *
 * The implementation uses a simple deterministic trial‑division test
 * for 64‑bit integers; the values 6*k±1 for the required k are well
 * within 64 bits for all n that fit in the output type.
 */
#include <stdint.h>
#include <stdbool.h>

/* simple deterministic primality test for uint64_t values   */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    if (n % 3 == 0)           return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k     = 0;          /* candidate k, start from 1 */
    while (count < n) {
        ++k;
        uint64_t v1 = 6ULL * (uint64_t)k - 1;   /* 6*k-1 */
        uint64_t v2 = 6ULL * (uint64_t)k + 1;   /* 6*k+1 */
        if (is_prime_u64(v1) && !is_prime_u64(v2))
            ++count;
    }
    return (seq_t)k;
}
