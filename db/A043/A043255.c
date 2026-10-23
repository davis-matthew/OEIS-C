/*
 * A043255  Numbers k such that 6 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  Numbers k such that 6 and 7 occur juxtaposed in the base‑10
   representation of k but not of k‑1.
   a(1) = 67, a(2) = 76, a(3) = 167, … */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* computed sequence values */
static size_t cache_len = 0;         /* allocated capacity                         */
static idx_t  max_computed = 0;      /* highest index that has been computed       */
static seq_t  last_k = 0;            /* last number that satisfied the condition   */

/* return true if x contains the decimal digits “67” or “76” as a pair */
static bool has_pair(seq_t x)
{
    int prev = -1;
    seq_t ten = (seq_t)10;
    while (x > (seq_t)0) {
        int d = (int)(x % ten);
        if (prev != -1 && ((prev == 6 && d == 7) || (prev == 7 && d == 6)))
            return true;
        prev = d;
        x /= ten;
    }
    return false;
}

/* extend the cache up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed)
        return;

    /* create initial buffer if needed */
    if (cache == NULL) {
        cache_len = 64;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* find the next k satisfying the condition */
        while (true) {
            ++last_k;
            if (has_pair(last_k) && !has_pair(last_k - 1))
                break;
        }

        /* grow buffer if necessary */
        if (cache_len <= (size_t)i - 1) {
            size_t new_len = cache_len * 2;
            if (new_len < (size_t)i)
                new_len = i;
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            cache_len = new_len;
        }
        cache[i - 1] = last_k;
    }
    max_computed = n;
}

extern seq_t a(idx_t n)
{
    /* Any n >= 1 is valid. Behaviour for n < 1 is unspecified. */
    ensure_cache(n);
    return cache[n - 1];
}
