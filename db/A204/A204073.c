/*
 * A204073  Number of (n+1)X6 0..2 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A204073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is given by the empirical linear recurrence of order 13
 * 
 *   a(n) = 78*a(n-1) -1521*a(n-2) +7304*a(n-3) +32884*a(n-4)
 *         -338760*a(n-5) +639312*a(n-6) +947424*a(n-7)
 *         -3942080*a(n-8) +2598528*a(n-9) +1739776*a(n-10)
 *         -1832448*a(n-11) -14336*a(n-12) +163840*a(n-13)
 * 
 *  Initial values:
 *  a(1) = 4688
 *  a(2) = 221726
 *  a(3) = 11081828
 *  a(4) = 563880962
 *  a(5) = 28864215128
 *  a(6) = 1480470688070
 *  a(7) = 75985220860460
 *  a(8) = 3900809853901802
 *  a(9) = 200268432775804640
 *  a(10) = 10282077911857180910
 *  a(11) = 527901424170345081716
 *  a(12) = 27103536327800063656274
 *  a(13) = 1391552121866875493481704
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

/* static cache for computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity in number of elements */
static size_t next_compute = 14;   /* next index to compute */

/* recurrence coefficients (a(n-1) to a(n-13)) */
static const int coeff[13] = {
    78,          /* a(n-1)  */
   -1521,        /* a(n-2)  */
    7304,        /* a(n-3)  */
   32884,        /* a(n-4)  */
  -338760,        /* a(n-5)  */
  639312,        /* a(n-6)  */
  947424,        /* a(n-7)  */
 -3942080,        /* a(n-8)  */
 2598528,        /* a(n-9)  */
 1739776,        /* a(n-10) */
 -1832448,        /* a(n-11) */
  -14336,        /* a(n-12) */
  163840          /* a(n-13) */
};

/* initialise the cache with the first 13 terms */
static void init_cache(idx_t n)
{
    if (!cache) {
        size_t cap = 14;                     /* indices 0..13 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        /* ignore index 0 */
        cache[1]  = seq_from_str("4688");
        cache[2]  = seq_from_str("221726");
        cache[3]  = seq_from_str("11081828");
        cache[4]  = seq_from_str("563880962");
        cache[5]  = seq_from_str("28864215128");
        cache[6]  = seq_from_str("1480470688070");
        cache[7]  = seq_from_str("75985220860460");
        cache[8]  = seq_from_str("3900809853901802");
        cache[9]  = seq_from_str("200268432775804640");
        cache[10] = seq_from_str("10282077911857180910");
        cache[11] = seq_from_str("527901424170345081716");
        cache[12] = seq_from_str("27103536327800063656274");
        cache[13] = seq_from_str("1391552121866875493481704");
        cache_cap = cap;
        next_compute = 14;
    }
    /* grow cache if necessary */
    size_t needed = (size_t)n + 2;   /* need index n */
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = needed;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* undefined for n < 1, return 0 */
    }
    init_cache(n);
    if (n < next_compute) {
        return cache[n];
    }
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 13; ++k) {
            idx_t idx = i - 1 - k;           /* a(i-1-k) */
            seq_t prod = cache[idx] * (seq_t)abs(coeff[k]);
            if (coeff[k] >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        cache[i] = sum_pos - sum_neg;
    }
    next_compute = n + 1;
    return cache[n];
}
