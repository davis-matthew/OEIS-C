/*
 * A223796  Number of nX3 0..3 arrays with rows and antidiagonals unimodal.
 * https://oeis.org/A223796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 50*a(n-1) -253*a(n-2) -1283*a(n-3) +8456*a(n-4)
          -10020*a(n-5) -12064*a(n-6) +27360*a(n-7)
          -6656*a(n-8) -9728*a(n-9) +4096*a(n-10)
   for n > 10,
   with initial values
   a(1)=50, a(2)=2500, a(3)=110116, a(4)=4816168,
   a(5)=210163664, a(6)=9169032476, a(7)=400006582368,
   a(8)=17450517286804, a(9)=761287955888788,
   a(10)=33211580867804324. */

#include <stdbool.h>

static const seq_t init_vals[10] = {
    (seq_t)50ULL,
    (seq_t)2500ULL,
    (seq_t)110116ULL,
    (seq_t)4816168ULL,
    (seq_t)210163664ULL,
    (seq_t)9169032476ULL,
    (seq_t)400006582368ULL,
    (seq_t)17450517286804ULL,
    (seq_t)761287955888788ULL,
    (seq_t)33211580867804324ULL
};

#define CACHE_MAX 10000

static seq_t cache[CACHE_MAX + 1];
static bool cached[CACHE_MAX + 1];
static idx_t last_computed = 0;

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined for n=0 */
    }

    if (n <= CACHE_MAX) {
        if (!cached[n]) {
            /* Initialize cache with the first ten values. */
            if (n <= 10) {
                cache[n] = init_vals[n - 1];
            } else {
                /* Ensure all previous terms up to n are computed. */
                for (idx_t i = last_computed + 1; i <= n; ++i) {
                    seq_t val;
                    if (i <= 10) {
                        val = init_vals[i - 1];
                    } else {
                        val = 50    * cache[i - 1]      - 253   * cache[i - 2]
                              - 1283 * cache[i - 3]      + 8456  * cache[i - 4]
                              - 10020 * cache[i - 5]     - 12064 * cache[i - 6]
                              + 27360 * cache[i - 7]     - 6656  * cache[i - 8]
                              - 9728  * cache[i - 9]     + 4096  * cache[i -10];
                    }
                    cache[i] = val;
                    cached[i] = true;
                }
            }
            last_computed = n;
        }
        return cache[n];
    }

    /* For n > CACHE_MAX, compute without storing all values.  */
    seq_t prev[10];
    for (idx_t i = 0; i < 10; ++i) {
        prev[i] = init_vals[i];
    }

    for (idx_t i = 11; i <= n; ++i) {
        seq_t val = 50    * prev[0]
                  - 253   * prev[1]
                  - 1283  * prev[2]
                  + 8456  * prev[3]
                  - 10020 * prev[4]
                  - 12064 * prev[5]
                  + 27360 * prev[6]
                  - 6656  * prev[7]
                  - 9728  * prev[8]
                  + 4096  * prev[9];
        /* Shift left: remove oldest term and append new. */
        for (idx_t j = 0; j < 9; ++j) {
            prev[j] = prev[j + 1];
        }
        prev[9] = val;
    }
    return prev[9];
}
