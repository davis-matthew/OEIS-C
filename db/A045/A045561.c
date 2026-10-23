/*
 * A045561  Numbers whose factorial has '52' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 3324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth positive integer m such that, after removing all trailing zeros,
   the last two digits of m! are 52.  In other words
   ((m! / 10^k) mod 100) == 52, where k is the number of trailing zeros of m!.
   The sequence produced is OEIS A045561. */

#include <stdint.h>
#include <stdlib.h>

static inline uint64_t pow2_mod_uint64(uint64_t exp)
{
    uint64_t result = 1;
    uint64_t base   = 2;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % 100;
        base = (base * base) % 100;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; defensive guard */
        return 0;

    /* static state survives between calls */
    static idx_t    cur          = 1;   /* last tested integer              */
    static uint64_t prod_mod_100 = 1;   /* product of all factors (excluding 2 and 5) modulo 100 */
    static uint64_t exp_2_minus_5 = 0;   /* net exponent of 2 minus exponent of 5   */
    static seq_t   *cache        = NULL;
    static size_t  cache_cap    = 0;
    static size_t  cache_len    = 0;

    while (cache_len < (size_t)n) {
        cur++;   /* test next integer */

        /* strip factors of 2 and 5 from cur */
        uint64_t x     = cur;
        uint64_t cnt2  = 0;
        while ((x & 1ULL) == 0) {
            x >>= 1;
            ++cnt2;
        }

        uint64_t cnt5  = 0;
        while (x % 5ULL == 0) {
            x /= 5ULL;
            ++cnt5;
        }

        /* update product modulo 100 (remove 2s and 5s from x) */
        prod_mod_100 = (prod_mod_100 * (x % 100ULL)) % 100ULL;

        /* update exponent difference */
        exp_2_minus_5 += cnt2;
        exp_2_minus_5 -= cnt5;   /* exp_2_minus_5 stays non‑negative for factorials */

        /* compute last two digits after removing trailing zeros */
        uint64_t val = (prod_mod_100 * pow2_mod_uint64(exp_2_minus_5)) % 100ULL;

        if (val == 52ULL) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failed – undefined behavior for this
                              context, but keep simple – we abort process */
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;   /* store the integer */
        }
    }

    return cache[(size_t)n - 1];
}
