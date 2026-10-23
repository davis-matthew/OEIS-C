/*
 * A025547  Least common multiple of {1,3,5,...,2n-1}.
 * https://oeis.org/A025547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = least common multiple of {1,3,5,…,2n-1}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) */
#include "../utils/seq_mul.h"            /* seq_t seq_mul(seq_t a, seq_t b) */

static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static idx_t    prime_lim  = 0;

seq_t a(idx_t n)
{
    idx_t limit = (idx_t)2 * n - 1;

    /* Ensure we have a list of all primes up to `limit`. */
    if (limit > prime_lim)
    {
        free(prime_cache);
        build_primes_upto(limit, &prime_cache, &prime_cnt);
        prime_lim = limit;
    }

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_cache[i];
        if (p == 2)  /* skip the even prime */
            continue;

        uint64_t pow = p;
        while (pow * p <= limit)  /* keep the highest power <= limit */
            pow *= p;

        result = seq_mul(result, (seq_t)pow);
    }
    return result;
}
