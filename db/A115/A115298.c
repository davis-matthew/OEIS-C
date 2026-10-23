/*
 * A115298  Row sums of A115297.
 * https://oeis.org/A115298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=2..n+1} (p_k - p_{ceil(k/2)})
 * Range:     u8 n <= 15, u16 n <= 202, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p_k denotes the k-th prime. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache = NULL;
static size_t   prime_cache_count = 0;

/* external linkage as required */
seq_t a(idx_t n)
{
    /* Ensure we have at least (n+1)-th prime cached */
    idx_t needed_index = n + 1;

    if (prime_cache == NULL || prime_cache_count <= needed_index) {
        /* Compute an upper bound for the (n+1)-th prime */
        idx_t limit;
        if (needed_index < 6) {
            limit = 15;                          /* 15 > p_6 = 13 */
        } else {
            double nd = (double)needed_index;
            double l  = nd * (log(nd) + log(log(nd))) + 3.0;
            limit = (idx_t)l;
        }

        /* Reallocate prime array if needed */
        free(prime_cache);
        prime_cache = NULL;
        size_t c;
        build_primes_upto(limit, &prime_cache, &c);
        prime_cache_count = c;
    }

    /* Accumulate the sum */
    seq_t sum = 0;
    for (idx_t k = 2; k <= n + 1; ++k) {
        seq_t pk      = (seq_t)prime_cache[k - 1];
        idx_t half    = (k + 1) / 2;            /* ceil(k/2) */
        seq_t p_half  = (seq_t)prime_cache[half - 1];
        sum += pk - p_half;
    }

    return sum;
}
