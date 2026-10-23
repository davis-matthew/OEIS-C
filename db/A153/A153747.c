/*
 * A153747  Numbers k such that there are 9 digits in k^2 and for each factor
 *          f of 9 (1,3) the sum of digit groupings of size f is a square.
 * https://oeis.org/A153747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 474, u64 n <= 474, u128 n <= 474,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A153747: 
   a(n) is the n‑th integer k such that
       1) k² has exactly 9 decimal digits,
       2) the sum of the nine digits of k² is a perfect square,
       3) writing k² as three consecutive 3‑digit blocks (abc, def, ghi),
          the sum abc + def + ghi is a perfect square.
   The sequence is indexed from 1.
*/

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"   /* provides seq_alloc(size_t) for allocating seq_t arrays */

static bool sq_digit[82];        /* flags for digit sums 0..81 which are squares   */
static bool sq_group[3000];      /* flags for group sums 0..2997 which are squares */
static seq_t *cache = NULL;      /* array of qualifying k values                 */
static size_t cache_len = 0;     /* number of stored values                      */
static bool init_done = false;   /* flag to run init only once                   */

/* Precompute the sieve of valid k values in [10000,31622] */
static void init_table(void)
{
    if (init_done) return;
    init_done = true;

    /* Mark perfect squares for digit and group sums */
    for (int i = 0; i * i <= 81; ++i)      sq_digit[i * i] = true;
    for (int i = 0; i * i <= 2997; ++i)   sq_group[i * i] = true;

    /* Allocate enough space for the worst‑case number of candidates */
    cache = seq_alloc(21623);   /* number of integers between 10000 and 31622 inclusive */
    cache_len = 0;

    const idx_t start = 10000;
    const idx_t end   = 31622;
    const seq_t div3  = 1000;           /* 10³  */
    const seq_t div6  = div3 * div3;    /* 10⁶  */

    for (idx_t k = start; k <= end; ++k)
    {
        seq_t sq = (seq_t)k * (seq_t)k;

        /* Sum of the 9 digits of sq */
        seq_t tmp = sq;
        int dsum = 0;
        while (tmp > 0) {
            dsum += (int)(tmp % 10);
            tmp /= 10;
        }
        if (!sq_digit[dsum]) continue;

        /* Sum of the three 3‑digit groups */
        seq_t g1 = sq / div6;                 /* most significant 3 digits */
        seq_t g2 = (sq / div3) % div3;        /* middle 3 digits         */
        seq_t g3 = sq % div3;                 /* least significant 3 digits */
        int gsum = (int)(g1 + g2 + g3);
        if (!sq_group[gsum]) continue;

        /* Store qualifying k */
        cache[cache_len++] = (seq_t)k;
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    init_table();                     /* ensure cache built once */
    if (n == 0 || n > cache_len)      /* out of bounds */
        return 0;
    return cache[n - 1];
}
