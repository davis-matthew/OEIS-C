/*
 * A061376  a(n) = f(n) + f(f(n)) where f(n) = 0 if n <= 1 or a prime,
 *          otherwise f(n) = sum of distinct primes dividing n.
 * https://oeis.org/A061376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f(n) + f(f(n)) where f(n) = 0 if n <= 1 or n is prime,
 *            otherwise f(n) = sum of distinct primes dividing n.
 *            Implementation: factorize each composite n, sum distinct primes,
 *            then evaluate f again on the result (which is a small number)
 * Range:     u8 n <= 397, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 96 terms through n = 9753 (2026-11-03)
 * Helpers:   factorize, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factorize.h"
#include "../utils/is_prime_u64.h"

static seq_t f_val(idx_t n)
{
    uint64_t x = (uint64_t)n;
    if (x <= 1 || is_prime_u64(x))
        return 0;

    /* Enough space for all distinct prime factors of a 64‑bit number. */
    uint64_t primes[16];
    int exps[16];
    size_t cnt = factorize(x, primes, exps);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];
    return sum;
}

seq_t a(idx_t n)
{
    seq_t fn = f_val(n);
    seq_t ff = f_val((idx_t)fn);
    return fn + ff;
}
