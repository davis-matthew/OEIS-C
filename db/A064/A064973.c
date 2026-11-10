/*
 * A064973  Values for which A065358 is larger than any preceding term.
 * https://oeis.org/A064973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A065358(k) = n, where A065358(k) =
 *            Sum_{i=1..k} (-1)^pi(i); computed by a sieve and walk (steps are
 *            +-1, so records are consecutive values 1,2,3,...)
 * Range:     u8 n <= 44, u16 n <= 617, u32 n <= 940, u64 n <= 940,
 *            u128 n <= 940, big any n
 * Validated: standard, pass, 70 terms through n = 871 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t *hits_arr = NULL;   /* hits_arr[h] = first k where ladder reaches h */
static idx_t hits_cnt = 0;       /* heights 1..hits_cnt known */
static idx_t hits_cap = 0;

static void build_hits(idx_t limit)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!comp) exit(1);
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                comp[j] = 1;
    hits_cnt = 0;
    long long s = 0, best = 0;
    int par = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        if (k >= 2 && !comp[k]) par ^= 1;
        s += par ? -1 : 1;
        if (s > best) {
            best = s;
            if ((idx_t)best + 1 > hits_cap) {
                hits_cap = hits_cap ? hits_cap * 2 : 1024;
                while (hits_cap < (idx_t)best + 1) hits_cap *= 2;
                hits_arr = (idx_t *)realloc(hits_arr, (size_t)hits_cap * sizeof(idx_t));
                if (!hits_arr) exit(1);
            }
            hits_arr[best] = k;
            hits_cnt = (idx_t)best;
        }
    }
    free(comp);
}

static idx_t cur_limit = 0;

seq_t a(idx_t n)
{
    while (n > hits_cnt) {
        cur_limit = cur_limit ? cur_limit * 2 : 4096;
        build_hits(cur_limit);
    }
    return (seq_t)hits_arr[n];
}
