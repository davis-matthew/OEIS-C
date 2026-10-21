/*
 * A092981  Least product of successive primes beginning from just greater
 *          than n which is > n!.
 * https://oeis.org/A092981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least product of successive primes starting from the *
 *            first prime > n that is greater than n! (OEIS A092981)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   next_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) {                 /* offset 1: behaviour below is irrelevant */
        return 0;
    }

    /* compute n! as seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact = seq_mul(fact, (seq_t)i);
    }

    /* first prime strictly greater than n */
    uint64_t p = next_prime(n);
    seq_t prod = 1;

    while (1) {
        prod = seq_mul(prod, (seq_t)p);   /* multiply by current prime */
        if (prod > fact) {
            break;                       /* product exceeds n! */
        }
        p = next_prime(p);                /* next successive prime */
    }

    return prod;
}
