/*
 * A122772  Numbers k, excluding powers of 2, such that a regular k-sided
 *          polygon can be constructed with a ruler and compass.
 * https://oeis.org/A122772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute OEIS A122772:
   generate all numbers of the form f·2ᵉ, where
   f is a product of distinct Fermat primes (3, 5, 17, 257, 65537),
   f ≠ 1.  The sequence is returned sorted; powers of two
   (f = 1) are excluded.  A tiny cache is kept so subsequent calls
   are fast.  The implementation works for arbitrary‑precision
   seq_t as well as for native integer sizes. */

static const seq_t f[31] = {
    (seq_t)3ULL,        (seq_t)5ULL,        (seq_t)15ULL,       (seq_t)17ULL,
    (seq_t)51ULL,       (seq_t)85ULL,        (seq_t)255ULL,      (seq_t)257ULL,
    (seq_t)771ULL,      (seq_t)1285ULL,      (seq_t)3855ULL,     (seq_t)4369ULL,
    (seq_t)13107ULL,    (seq_t)21845ULL,     (seq_t)65535ULL,    (seq_t)65537ULL,
    (seq_t)196611ULL,   (seq_t)327685ULL,    (seq_t)983055ULL,   (seq_t)1114129ULL,
    (seq_t)3342387ULL,  (seq_t)5570645ULL,   (seq_t)16711935ULL, (seq_t)16843009ULL,
    (seq_t)50529027ULL, (seq_t)84215045ULL,  (seq_t)252645135ULL, (seq_t)286331153ULL,
    (seq_t)858993459ULL, (seq_t)1431655765ULL, (seq_t)4294967295ULL
};

static seq_t cur[31];
static bool cur_initialized = false;

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

seq_t a(idx_t n)
{
    if (!cur_initialized) {
        for (int i = 0; i < 31; ++i)
            cur[i] = f[i];
        cur_initialized = true;
    }

    while (cache_len < (size_t)n) {
        /* find the smallest current value */
        size_t min_i = 0;
        seq_t min_val = cur[0];
        for (size_t i = 1; i < 31; ++i) {
            if (cur[i] < min_val) {
                min_val = cur[i];
                min_i = i;
            }
        }

        /* grow cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* insufficient memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = min_val;
        cur[min_i] = cur[min_i] * (seq_t)2;   /* advance that sequence */
    }

    return cache[n - 1];
}
