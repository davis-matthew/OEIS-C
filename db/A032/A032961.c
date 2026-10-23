/*
 * A032961  Numbers whose base-11 representation Sum_{i=0..m} d(i)*11^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A032961: numbers whose base‑11 representation has
 * even digits at all odd positions (counting from 0, the most
 * significant digit).  The algorithm checks candidates sequentially
 * and caches the first n found values for fast repeated queries. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *cache = NULL;      /* array of computed terms             */
static idx_t  cache_len = 0;    /* number of entries currently stored */
static idx_t  cache_cap = 0;    /* current allocated capacity         */
static idx_t  candidate = 1;    /* next integer to test               */

/* test whether a number satisfies the parity rule in base 11 */
static bool good(idx_t n)
{
    /* store remainders of division by 11 */
    uint32_t digits[32];
    idx_t len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (n > 0) {
            digits[len++] = n % 11U;
            n /= 11U;
        }
    }

    /* iterate from most significant digit (index i = len-1) to least */
    for (idx_t pos = 0; pos < len; ++pos) {
        idx_t i = len - 1 - pos;   /* i is the position from the left */
        if (i & 1U) {              /* odd position            */
            if (digits[pos] & 1U) /* digit is odd             */
                return false;
        }
    }
    return true;
}

/* ensure the cache can hold at least 'needed' elements */
static void grow_cache_if_needed(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1);                 /* allocation failed */

    cache = new_ptr;
    cache_cap = new_cap;
}

/* public sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; undefined for n < 1 */
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        if (good(candidate)) {
            grow_cache_if_needed(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
