/*
 * A060386  In base ten, we try to get the digits 0,1,2,3,...,9,0,1,2,3,... in
 *          order only using primes and each time choosing the smallest prime
 *          that will give the desired digit.
 * https://oeis.org/A060386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime whose decimal representation contains the
 *            digit (n mod 10)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   contains_digit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/contains_digit_uint64.h"

/* Basic prime test for numbers that fit in 64 bits.
   It is adequate because the primes we search for are very small. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Find the smallest prime that contains the digit d (0 ≤ d ≤ 9). */
static seq_t smallest_prime_with_digit(int d) {
    for (uint64_t n = 2; ; ++n) {
        if (is_prime_uint64(n) && contains_digit_uint64(n, d)) {
            return (seq_t)n;
        }
    }
}

/* Memoised lookup table of the required primes, one per desired digit. */
static seq_t digit_prime[10];
static bool table_ready = false;

/* Returned value for a(n) with 0‑based offset. */
seq_t a(idx_t n) {
    if (!table_ready) {
        for (int d = 0; d < 10; ++d)
            digit_prime[d] = smallest_prime_with_digit(d);
        table_ready = true;
    }
    return digit_prime[(int)(n % (idx_t)10)];
}
