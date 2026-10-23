/*
 * A166571  Prime numbers containing the string 10.
 * https://oeis.org/A166571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 232, u32 n <= 233, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_10, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search primes whose decimal representation contains the substring "10". */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/contains_10.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1, so a(1) is first term */
    seq_t candidate = 101;                /* first known prime with "10" */
    idx_t count = 0;
    while (1) {
        if (contains_10(candidate) && is_prime_big(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
