/*
 * A237582  a(n) = |{0 < k < n: pi(n + k^2) is prime}|, where pi(.) is given
 *          by A000720.
 * https://oeis.org/A237582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   _pi_le, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = #{0 < k < n | pi(n + k^2) is prime} */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_pi_le.h"

static idx_t _max_limit = 0;
static uint64_t *_primes = NULL;
static size_t _primes_cnt = 0;

seq_t a(idx_t n) {
    /* Ensure primes up to n + (n-1)^2 are available */
    idx_t limit = n + (n - 1) * (n - 1);
    if (limit > _max_limit) {
        free(_primes);
        build_primes_upto(limit, &_primes, &_primes_cnt);
        _max_limit = limit;
    }

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t x = n + k * k;
        size_t pi_val = _pi_le(_primes, _primes_cnt, x);
        if (is_prime((idx_t)pi_val))
            ++count;
    }
    return count;
}
