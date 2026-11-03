/*
 * A089583  Numbers n which are a multiple of A068505(n) (= n read in base m+1
 *          where m = largest digit of n).
 * https://oeis.org/A089583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 956 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A089583
 * a(n) = the n-th positive integer m such that m is a multiple of
 *        A068505(m) (i.e. m read in base (d+1) where d is the
 *        largest decimal digit of m).  The sequence is generated
 *        by a simple search.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* compute A068505 value of x : interpret decimal digits of x
 * in base (max_digit+1) and return the resulting integer
 */
static seq_t A068505(seq_t x)
{
    /* extract decimal digits (least significant first) */
    int digits[32];
    int cnt = 0;
    seq_t tmp = x;
    if (tmp == 0) {
        digits[cnt++] = 0;
    } else {
        while (tmp != 0) {
            digits[cnt++] = (int)(tmp % 10);
            tmp /= 10;
        }
    }

    /* find largest digit */
    int maxd = 0;
    for (int i = 0; i < cnt; ++i) {
        if (digits[i] > maxd)
            maxd = digits[i];
    }
    int base = maxd + 1;            /* base is between 1 and 10 */

    /* interpret digits as base-'base' number, most significant first */
    seq_t val = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        val = val * base + (seq_t)digits[i];
    }
    return val;
}

/* singleton cache to avoid recomputing sequence terms */
static seq_t *cache      = NULL;   /* array of a(k) values */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static size_t cache_len  = 0;      /* number of terms stored */

/* the current candidate number to test for inclusion in the sequence */
static seq_t current_candidate = 1;

/* add a new term to the cache, reallocating if necessary */
static void cache_add(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
        if (!new_arr)
            exit(EXIT_FAILURE);          /* memory exhausted */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* main sequence function as required by the specification */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n==0, return 0 */

    /* generate terms until we have the n-th term */
    while (cache_len < n) {
        seq_t val = A068505(current_candidate);
        if (val != 0 && current_candidate % val == 0)
            cache_add(current_candidate);
        ++current_candidate;
    }
    return cache[n - 1];
}
