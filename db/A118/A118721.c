/*
 * A118721  Chen primes for which the sum of the digits is also a Chen prime.
 * https://oeis.org/A118721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, digit_sum, factor_u64, is_prime,
 *            is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Chen primes for which the sum of the digits is also a Chen prime.
   The function walks from the first integer upward, checks the Chen
   prime property for the integer itself, and then for the sum of its
   decimal digits.  All results are cached statically. */
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t x);          */
#include "../utils/is_prime_small.h"    /* bool is_prime_small(seq_t n);    */
#include "../utils/digit_sum.h"         /* seq_t digit_sum(seq_t v);        */
#include "../utils/append_candidate.h"  /* void append_candidate(...);       */
#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t n,...); */

static bool isChenPrime64(uint64_t x)
{
    /* Chen prime: a prime p such that p+2 is prime or a product of two
       (not necessarily distinct) primes. */
    if (!is_prime(x)) return false;
    uint64_t y = x + 2;
    if (is_prime(y)) return true;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(y, primes, exps, &cnt);
    unsigned total = 0;
    for (size_t i = 0; i < cnt; ++i) total += exps[i];
    return total == 2;
}

seq_t a(idx_t n)
{
    static seq_t *list = NULL;
    static size_t list_len = 0;
    static size_t list_cap = 0;
    static idx_t current = 2;   /* next integer to test */

    if (n == 0) return 0;   /* safety; OEIS offset is 1 */

    while (list_len < n) {
        if (is_prime(current)) {
            uint64_t p = (uint64_t)current;
            if (isChenPrime64(p)) {
                seq_t sum = digit_sum((seq_t)current);
                uint64_t sum_u = (uint64_t)sum;
                if (isChenPrime64(sum_u)) {
                    append_candidate(&list, &list_len, &list_cap, (seq_t)current);
                }
            }
        }
        ++current;
    }
    return list[n - 1];
}
