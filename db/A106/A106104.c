/*
 * A106104  Primes with minimal digit = 4.
 * https://oeis.org/A106104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes whose decimal digits are all ≥4 and at least one digit is
 *            4
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   has_digit_ge4_and_four, is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime64.h"
#include "../utils/has_digit_ge4_and_four.h"

static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 1;

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    if (n <= cache_cnt)
        return cache[n - 1];

    if (cache_cap == 0)
    {
        cache_cap = 256;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    seq_t cand = last_checked;
    while (cache_cnt < n)
    {
        cand++;
        if (has_digit_ge4_and_four((uint64_t)cand))
        {
            if (is_prime64((uint64_t)cand))
            {
                if (cache_cnt == cache_cap)
                {
                    cache_cap *= 2;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = cand;
            }
        }
    }
    last_checked = cand;
    return cache[n - 1];
}
