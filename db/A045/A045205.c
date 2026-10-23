/*
 * A045205  Numbers whose base-5 representation contains no 0's and exactly
 *          three 4's.
 * https://oeis.org/A045205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains no 0's and exactly
 *            three 4's
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;   /* number of values stored in cache */

    /* static state for incremental generation */
    static idx_t cur_len  = 3;        /* current length of base‑5 digits */
    static uint64_t cur_index = 0;    /* current index within 4^cur_len combinations */
    static uint64_t cur_max = 64;     /* 4^cur_len */

    /* extend cache if required */
    if (cache_count < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t newcap = cache_cap ? cache_cap * 2 : 8;
            if (newcap < (size_t)n) newcap = n;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cache_cap = newcap;
        }

        /* generate terms until the first n are stored */
        while (cache_count < (size_t)n) {
            /* advance to next length when exhausted */
            if (cur_index == cur_max) {
                cur_len++;
                cur_index = 0;
                /* recompute cur_max = 4^cur_len */
                uint64_t pow4 = 1;
                for (idx_t i = 0; i < cur_len; ++i) pow4 *= 4ULL;
                cur_max = pow4;
            }

            /* convert cur_index to base‑4 digits and compute value */
            uint64_t temp = cur_index;
            int cnt4 = 0;
            int digits[32];  /* supports cur_len up to 32 safely */
            for (idx_t pos = 0; pos < cur_len; ++pos) {
                int d4 = temp & 3ULL;      /* base‑4 digit 0..3 */
                int d  = d4 + 1;           /* map to {1,2,3,4} */
                if (d == 4) ++cnt4;
                digits[(size_t)pos] = d;
                temp >>= 2;
            }

            seq_t val = 0;
            for (idx_t pos = cur_len; pos-- > 0; ) {
                val = val * 5 + (seq_t)digits[(size_t)pos];
            }

            if (cnt4 == 3) {
                cache[cache_count++] = val;
            }

            ++cur_index;
        }
    }

    return cache[(size_t)n - 1];
}
