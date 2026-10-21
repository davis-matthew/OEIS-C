/*
 * A075860  a(n) is the fixed point reached when the map x -> A008472(x) is
 *          iterated, starting from x = n, with the convention a(1)=0.
 * https://oeis.org/A075860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_distinct_prime_factors_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1, iterate x -> sum_of_distinct_prime_factors(x) until a prime occurs. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/sum_distinct_prime_factors_uint64.h"

/* Return true if n is a prime number (n >= 2). */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    uint64_t cur = (uint64_t)n;
    while (true) {
        if (is_prime_uint64(cur)) return (seq_t)cur;
        cur = sum_distinct_prime_factors_uint64(cur);
    }
}
