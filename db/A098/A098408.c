/*
 * A098408  a(1) = 1; for n>1, a(n) = smallest number > a(n-1) such that last
 *          digit of a(n-1) + first digit of a(n) = 3.
 * https://oeis.org/A098408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098408:
   a(1) = 1; for n>1, a(n) = smallest number > a(n-1)
   such that last digit of a(n-1) + first digit of a(n) = 3. */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/pow10.h"

static seq_t *cache   = NULL; /* terms stored at indices 1.. */
static idx_t  cache_cap = 0;   /* capacity (last usable index) */
static idx_t  cache_len = 0;   /* number of terms actually computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not part of the sequence */

    /* Expand cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n + 16;        /* small slack */
        seq_t *new_cache;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        }
        if (new_cache == NULL) return (seq_t)0; /* OOM – abort silently */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    while (cache_len < n) {
        ++cache_len;
        if (cache_len == 1) {          /* a(1) = 1 */
            cache[1] = (seq_t)1;
            continue;
        }

        seq_t prev = cache[cache_len - 1];

        /* last digit of previous term */
        int last_digit = (int)(prev % 10);   /* 0‑9 */

        /* first digit needed in next term */
        int first_digit = 3 - last_digit;    /* will be 2 or 3 */

        /* count digits of prev */
        int digits = 0;
        seq_t tmp = prev;
        do {
            tmp /= 10;
            ++digits;
        } while (tmp > 0);

        /* 10^(digits-1) : the smallest number with 'digits' decimal digits */
        seq_t powBase = pow10((idx_t)(digits - 1));

        /* smallest number of same length starting with first_digit */
        seq_t base_f = (seq_t)first_digit * powBase;

        seq_t candidate;
        if (base_f > prev) {
            candidate = base_f;          /* already larger */
        } else {
            seq_t suffix = prev - base_f + 1;
            if (suffix < powBase) {
                candidate = base_f + suffix;          /* still same length */
            } else {
                /* need next length (increase digits by 1) */
                candidate = (seq_t)first_digit * pow10((idx_t)digits);
            }
        }

        cache[cache_len] = candidate;
    }

    return cache[n];
}
