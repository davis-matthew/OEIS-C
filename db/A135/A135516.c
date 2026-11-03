/*
 * A135516  a(0)=1; a(n) = (Product_{i=1..n} prime(i)^2) - 1, where prime(i)
 *          is the i-th prime.
 * https://oeis.org/A135516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (Product_{i=1..n} prime(i)^2) - 1, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 92 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    if (n == 0) {
        return seq_one();
    }

    static idx_t cached_max_n = 0;
    static size_t prime_cnt = 0;
    static uint64_t *prime_arr = NULL;

    if (n > cached_max_n) {
        if (prime_arr) {
            free(prime_arr);
            prime_arr = NULL;
        }

        idx_t limit = _prime_limit_for_idx(n);
        build_primes_upto(limit, &prime_arr, &prime_cnt);

        cached_max_n = n;
    }

    seq_t prod = seq_one();

    for (idx_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)prime_arr[i];
        prod = prod * p * p;
    }

    return prod - (seq_t)1;
}
