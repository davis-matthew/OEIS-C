/*
 * A175663  Maximal run length of primes of the form n, n+2, n+2*3, n+2*3*5,..
 * https://oeis.org/A175663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 40, u32 n <= 40, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A175663: a(n) is the maximal run length of primes in
    n, n+2, n+2·3, n+2·3·5, n+2·3·5·7, … (product of the first k primes). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static const uint32_t _primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u
};
static const size_t _primes_len = sizeof(_primes) / sizeof(_primes[0]);

seq_t a(idx_t n) {
    seq_t base = (seq_t)n;
    seq_t count = 0;

    /* check the first term n itself */
    if (!is_prime_big(base))
        return 0;
    ++count;    /* n is prime */

    seq_t prod = 1;   /* product of the first k primes */
    for (size_t i = 0; i < _primes_len; ++i) {
        prod = prod * (seq_t)_primes[i];   /* prod = 2 * 3 * ... * p_i */
        seq_t term = base + prod;
        if (!is_prime_big(term))
            break;
        ++count;
    }
    return count;
}
