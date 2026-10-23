/*
 * A044411  Numbers n such that string 7,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044411:  numbers n such that the decimal string "79"
   occurs in n but does not occur in n-1. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms   */
static size_t cache_cap = 0;     /* capacity of array         */
static size_t cache_used = 0;    /* number of terms stored    */
static seq_t  next_val = 0;      /* next number to test       */

/* Return true if decimal representation of v contains the substring "79". */
static int contains79(seq_t v)
{
    int prev = -1;          /* previous digit in reverse order   */
    while (v > 0) {
        int d = (int)(v % 10);
        if (prev == 9 && d == 7) return 1;
        prev = d;
        v /= 10;
    }
    return 0;
}

static void ensure_cache_capacity(void)
{
    if (cache_used >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n>=1 for valid queries */
    /* compute terms until we reach n */
    while (cache_used < n) {
        next_val += 1u;
        if (contains79(next_val) && !contains79(next_val - 1u)) {
            ensure_cache_capacity();
            cache[cache_used++] = next_val;
        }
    }
    return cache[n - 1];
}
