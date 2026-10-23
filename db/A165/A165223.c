/*
 * A165223  Products of 2 successive Mersenne primes.
 * https://oeis.org/A165223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000668(n) * A000668(n+1) (product of two successive
 *            Mersenne primes)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "../utils/pow2.h"

/* Exponents p for which 2^p − 1 is known to be prime (Mersenne primes). */
static const uint64_t mersenne_exponents[] = {
    2,   3,   5,   7,   13,  17,  19,  31,  61,  89,
    107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423,
    9689, 9941, 11213, 19937, 21701, 23209, 25733, 35943,
    42561, 45841, 49369, 71941, 85957, 122403, 125011,
    134011, 138001, 142085
};
static const size_t mersenne_exponents_count =
    sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

seq_t a(idx_t n)
{
    /* a(1) uses the first two exponents, a(2) the next two, and so on. */
    assert(n >= 1);

    /* Guard against out‑of‑range requests; such calls are not expected in tests. */
    if ((size_t)n + 1 > mersenne_exponents_count) {
        return 0;
    }

    seq_t m1 = pow2((idx_t)mersenne_exponents[n - 1]) - (seq_t)1;
    seq_t m2 = pow2((idx_t)mersenne_exponents[n])     - (seq_t)1;
    return m1 * m2;
}
