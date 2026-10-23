/*
 * A175940  Number of ways of writing n=p+f with p a prime and f a factorial.
 * https://oeis.org/A175940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = p + f with p prime and f
 *            factorial
 * Range:     u8 n <= 5548, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Precomputed factorials up to 7! (5040), sufficient for n <= 10000. */
static const seq_t factorials[] = {
    (seq_t)1ULL,
    (seq_t)2ULL,
    (seq_t)6ULL,
    (seq_t)24ULL,
    (seq_t)120ULL,
    (seq_t)720ULL,
    (seq_t)5040ULL
};

extern seq_t a(idx_t n)
{
    /* n is 1-based index; behaviour for n < 1 is undefined. */
    seq_t count = 0;

    for (size_t i = 0; i < sizeof(factorials)/sizeof(factorials[0]); ++i)
    {
        seq_t f = factorials[i];
        /* factorial larger than n → stop. */
        if (f > (seq_t)n) break;

        idx_t p = n - (idx_t)f;
        if (p >= 2 && is_prime(p))
            count += 1;
    }
    return count;
}
