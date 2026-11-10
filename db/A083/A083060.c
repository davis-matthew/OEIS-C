/*
 * A083060  a(n) is the number of natural numbers k such that A078496(k) =
 *          prime(n), where prime(n) denotes the n-th prime.
 * https://oeis.org/A083060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{k : A078496(k) = prime(n)}; computed by search over k
 *            in [(p+2)/2, p-1], p = prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8010 (2026-11-07)
 * Helpers:   is_prime, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/nth_prime.h"

static idx_t A083060_A078496(idx_t n)
{
    if (n < 4)
        return 0;
    idx_t limit = 2 * n - 2;
    for (idx_t p = n + 1; p <= limit; ++p) {
        if (is_prime(p) && is_prime(2 * n - p))
            return p;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t p = (idx_t)nth_prime(n);
    idx_t lo = (p + 2) / 2;
    if (lo < 4)
        lo = 4;
    idx_t count = 0;
    for (idx_t k = lo; k < p; ++k) {
        if (A083060_A078496(k) == p)
            ++count;
    }
    return (seq_t)count;
}
