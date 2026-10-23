/*
 * A038187  Numbers other than powers of 10 that are coprime to the sum of
 *          their digits.
 * https://oeis.org/A038187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_sum, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038187: Numbers other than powers of 10 that are coprime to the sum of their digits.
   Computed by scanning integers, skipping powers of 10, and selecting those for which
   gcd(n, sum_of_digits(n)) == 1.  Results are memoised so repeated calls are cheap. */

#include <stdlib.h>   /* for malloc, realloc, abort */
#include <stdbool.h>  /* for bool, true, false */

#include "../utils/gcd.h"
#include "../utils/digit_sum.h"

/* Test whether x is a power of 10 (i.e. 1, 10, 100, ...). */
static bool is_power_of_10(seq_t x)
{
    seq_t t = x;
    if (t == (seq_t)0)          /* 0 is not considered a power of 10 here */
        return false;
    while (t % (seq_t)10 == (seq_t)0) {
        t /= (seq_t)10;
        if (t == (seq_t)1)
            return true;
    }
    return t == (seq_t)1;
}

/* Static memoisation state */
static seq_t *cache      = NULL;   /* array of found terms */
static idx_t cache_size  = 0;      /* count of terms stored */
static size_t cache_cap  = 0;      /* current allocation capacity */
static seq_t next_candidate = (seq_t)1;  /* next integer to test */

/* Return the n-th term (n = 1 indicates the first term) */
seq_t a(idx_t n)
{
    /* Build cache until it contains at least n terms. */
    while (cache_size < n) {
        /* Skip powers of 10. */
        if (!is_power_of_10(next_candidate)) {
            seq_t sum = digit_sum(next_candidate);
            if (gcd(next_candidate, sum) == (seq_t)1) {
                /* Grow allocation if necessary. */
                if (cache_size == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();                 /* allocation failed */
                    cache = (seq_t *)tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = next_candidate;
            }
        }
        next_candidate += (seq_t)1;
    }

    /* cache is 0‑based; n is 1‑based. */
    return cache[n - (idx_t)1];
}
