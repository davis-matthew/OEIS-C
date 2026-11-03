/*
 * A051350  Sum of digit-sums of first n nonprimes.
 * https://oeis.org/A051350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=1..n} digit_sum(nonprime_k)
 * Range:     u8 n <= 37, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   digit_sum_u64_to_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* nonprime_k is the k-th positive integer that is not prime (1 is included). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum_u64_to_seq.h"
#include "../utils/is_prime_u64.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0U) return 0;

    seq_t total = 0;
    idx_t counted = 0;
    uint64_t current = 1;

    while (counted < n) {
        if (!is_prime_u64(current)) {
            total += digit_sum_u64_to_seq(current);
            counted++;
        }
        current++;
    }
    return total;
}
