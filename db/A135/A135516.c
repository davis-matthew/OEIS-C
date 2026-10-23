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
 * Helpers:   _prime_limit_for_idx, build_primes_upto, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_one.h"

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
