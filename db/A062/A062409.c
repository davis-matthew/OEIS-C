/*
 * A062409  Numbers k such that floor(e*k) is prime.
 * https://oeis.org/A062409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062409: Numbers k such that floor(e*k) is prime.
   This implementation searches k sequentially and counts how many
   produce a prime floor(e*k). */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_uint64.h"

static const long double E_CONST = 2.7182818284590452353602874713527L;

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n < 1, but return 0 */
        return (seq_t)0;

    idx_t count = 0;
    idx_t k = 0;

    while (count < n) {
        ++k;
        unsigned long long m = (unsigned long long)floorl((long double)k * E_CONST);
        if (is_prime_uint64(m))
            ++count;
    }
    return (seq_t)k;
}
