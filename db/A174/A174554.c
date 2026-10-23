/*
 * A174554  Smallest k > 2 such that 2|k, 3|k+1, 4|k+2,..., n|k+n-2.
 * https://oeis.org/A174554
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + lcm(2,3,4,...,n) (OEIS A174554)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_arr = NULL;      /* array of primes up to current max n */
static size_t prime_count = 0;          /* number of primes in prime_arr      */
static idx_t max_n = 0;                 /* largest n for which primes are known */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; values below the offset are irrelevant. */
    if (n < 2)
        return 0;

    /* Extend the prime table if this n is larger than what we have computed. */
    if (n > max_n) {
        if (prime_arr) {
            free(prime_arr);
            prime_arr = NULL;
        }
        build_primes_upto(n, &prime_arr, &prime_count);
        max_n = n;
    }

    /* Compute L = lcm(1,2,…,n) as a product of prime powers. */
    seq_t L = seq_one();                     /* start with 1 */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_arr[i];
        if (p > (uint64_t)n)
            break;                           /* primes beyond n don't contribute */

        /* Find the largest exponent e such that p^e ≤ n. */
        uint64_t prod = p;
        int exp = 0;
        while (prod <= (uint64_t)n) {
            ++exp;
            prod *= p;
        }

        /* Multiply L by p^exp. */
        seq_t pp = seq_pow((seq_t)p, (idx_t)exp);
        L = seq_mul(L, pp);
    }

    /* a(n) = 2 + L. The '+' operator works for seq_t of any supported size. */
    return L + (seq_t)2;
}
