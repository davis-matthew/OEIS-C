/*
 * A172488  Primes of the form 2^i * 3^j + 1 with i + j = 13.
 * https://oeis.org/A172488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes of the form 2^i * 3^j + 1 with i + j = 13,
   ordered increasingly. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cached list of the 5 primes.  The cache is built on first call. */
static seq_t *cache = NULL;
static size_t cache_count = 0;

static void build_cache(void)
{
    const int SUM = 13;
    const int TOTAL = SUM + 1;          /* i ranges 0..13 inclusive */
    cache = (seq_t *)malloc(TOTAL * sizeof(seq_t));
    cache_count = 0;

    for (int i = 0; i <= SUM; ++i) {
        int j = SUM - i;
        uint64_t val64 = (uint64_t)1 << i;          /* 2^i */
        for (int k = 0; k < j; ++k)
            val64 *= (uint64_t)3;                  /* * 3^j   */
        val64 += 1ULL;                              /* + 1     */
        seq_t val = (seq_t)val64;

        if (is_prime_big(val))
            cache[cache_count++] = val;
    }

    /* Sort ascending.  cache_count is at most 5.  */
    for (size_t p = 0; p + 1 < cache_count; ++p)
        for (size_t q = p + 1; q < cache_count; ++q)
            if (cache[p] > cache[q]) {
                seq_t tmp = cache[p];
                cache[p] = cache[q];
                cache[q] = tmp;
            }
}

seq_t a(idx_t n)
{
    if (n == 0 || n < 1)
        return 0;               /* behaviour below offset is irrelevant */

    if (cache == NULL)
        build_cache();

    if (n <= cache_count)
        return cache[n - 1];
    return 0;                   /* sequence is finite */
}
