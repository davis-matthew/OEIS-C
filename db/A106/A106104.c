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
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Check that all decimal digits are ≥4 and that a 4 occurs at least once. */
static inline bool has_digit_ge4_and_four(uint64_t v)
{
    bool has4 = false;
    while (v > 0)
    {
        uint8_t d = v % 10;
        if (d < 4) return false;
        if (d == 4) has4 = true;
        v /= 10;
    }
    return has4;
}

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
        cache = mem_new(seq_t, cache_cap);
    }

    seq_t cand = last_checked;
    while (cache_cnt < n)
    {
        cand++;
        if (has_digit_ge4_and_four((uint64_t)cand))
        {
            if (is_prime_u64((uint64_t)cand))
            {
                if (cache_cnt == cache_cap)
                {
                    cache_cap *= 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_cnt++] = cand;
            }
        }
    }
    last_checked = cand;
    return cache[n - 1];
}
