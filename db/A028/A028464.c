/*
 * A028464  Number of perfect matchings in graph C_{3} X C_{3} X P_{2n}.
 * https://oeis.org/A028464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028464:
   Number of perfect matchings in graph C3 × C3 × P_{2n}.
   The sequence satisfies the linear recurrence
      a(n) =  2759·a(n-1) - 325448·a(n-2) + 10121602·a(n-3)
             - 125374286·a(n-4) + 684055000·a(n-5) - 1622518920·a(n-6)
             + 1622518920·a(n-7) - 684055000·a(n-8) + 125374286·a(n-9)
             - 10121602·a(n-10) + 325448·a(n-11) - 2759·a(n-12) + a(n-13)
   for n ≥ 13 with initial values
      a(0)=1, a(1)=1120, a(2)=2861029, a(3)=7537209013, a(4)=19875272280736,
      a(5)=52411725012875905, a(6)=138211512392292291937,
      a(7)=364468498187098321751584, a(8)=961115930137025304064194421,
      a(9)=2534495671871264129163903449317, a(10)=6683552014192354263830206528781536,
      a(11)=17624755892139792658340655302347504609.
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache of computed sequence values */
static size_t cache_cap = 0;         /* current capacity of the cache */
static size_t cache_size = 0;        /* number of entries currently computed */

/* 13 recurrence coefficients r_i (from the denominator of the generating function) */
static const int64_t r_coeff[13] = {
    -2759, 325448, -10121602, 125374286,
    -684055000, 1622518920, -1622518920, 684055000,
    -125374286, 10121602, -325448, 2759, -1
};

/* Numerator coefficients N_k for k = 0..12 */
static const int64_t N_coeff[13] = {
    1, -1639, 96397, -1989840, 17959907,
    -72515033, 117260158, -72515033, 17959907,
    -1989840, 96397, -1639, 1
};

/* Ensure cache has capacity to store at least n+1 entries */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) using recurrence & stored values */
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (cache == NULL)
        grow_cache(n + 1);
    else if (n + 1 > cache_cap)
        grow_cache(n + 1);

    /* Compute missing terms */
    for (size_t k = cache_size; k <= (size_t)n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k <= 12) {
            seq_t sum_minus = 0, sum_plus = 0;
            for (int i = 1; i <= (int)k; ++i) {
                int64_t c = r_coeff[i - 1];
                if (c > 0) {
                    sum_plus += cache[k - i] * (seq_t)c;
                } else {
                    sum_minus += cache[k - i] * (seq_t)(-c);
                }
            }
            seq_t result = sum_minus;
            int64_t nk = N_coeff[(int)k];
            if (nk > 0) {
                result += (seq_t)nk;
            } else if (nk < 0) {
                result -= (seq_t)(-nk);
            }
            result -= sum_plus;
            cache[k] = result;
        } else {
            seq_t sum_minus = 0, sum_plus = 0;
            for (int i = 1; i <= 13; ++i) {
                int64_t c = r_coeff[i - 1];
                if (c > 0) {
                    sum_plus += cache[k - i] * (seq_t)c;
                } else {
                    sum_minus += cache[k - i] * (seq_t)(-c);
                }
            }
            cache[k] = sum_minus - sum_plus;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
