/*
 * A004680  Primes written in base 6.
 * https://oeis.org/A004680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007092(prime(n)) - convert the n-th prime to base 6
 * Range:     u8 n <= 28, u16 n <= 985, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   find_nth_prime, is_prime_helper, to_base6_as_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/find_nth_prime.h"
#include "../utils/is_prime_helper.h"
#include "../utils/to_base6_as_decimal.h"

seq_t a(idx_t n) {
    uint64_t prime = find_nth_prime(n);
    return to_base6_as_decimal(prime);
}
