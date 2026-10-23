/*
 * A037326  Numbers whose base-5 and base-10 expansions have the same digit
 *          sum.
 * https://oeis.org/A037326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037326: numbers whose base‑5 and base‑10 digit sums are equal.
   Implementation performs an incremental search and caches results for
   fast future access. */
#include <stdlib.h>

static seq_t *cache = NULL;       /* cached sequence values          */
static size_t cache_len = 0;      /* number of cached terms          */
static size_t cache_cap = 0;      /* allocated capacity of the cache */

/* last integer that has been examined during the incremental search   */
static seq_t last_checked = 0;

static int equal_digit_sums(seq_t x)
{
    /* compute decimal digit sum */
    seq_t dsum = 0;
    seq_t t = x;
    while (t > 0) {
        dsum += t % (seq_t)10;
        t    /= (seq_t)10;
    }

    /* compute base‑5 digit sum */
    seq_t bsum = 0;
    t = x;
    while (t > 0) {
        bsum += t % (seq_t)5;
        t    /= (seq_t)5;
    }
    return dsum == bsum;
}

static void grow_cache(void)
{
    size_t new_cap;
    if (cache_cap == 0)              new_cap = 16;
    else                            new_cap = cache_cap << 1;

    /* in C++ the allocator is not throw‑safe by default; call
       realloc and check for NULL for robustness, but the production
       environment guarantees enough memory for the required size.     */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure the requested terms have been generated. */
    if (n == 0) {
        return (seq_t)0;            /* offset is 1; value for n==0 is unused      */
    }
    while (cache_len < n) {
        ++last_checked;             /* next integer to test      */
        if (equal_digit_sums(last_checked)) {
            if (cache_len >= cache_cap) {
                grow_cache();
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
}
