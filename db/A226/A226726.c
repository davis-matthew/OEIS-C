/*
 * A226726  Numbers n such that the product of its decimal digits (A007954) is
 *          between n/2-1 and n/2+1.
 * https://oeis.org/A226726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A226726: numbers n whose product of decimal digits
   is strictly between n/2‑1 and n/2+1.  For even n the product must equal
   n/2; for odd n it must equal ⌊n/2⌋ or ⌊n/2⌋+1.  The function returns
   the nth term of this sequence. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_product.h"

static seq_t *cache = NULL;    /* cached terms of the sequence   */
static size_t cache_cap = 0;   /* current capacity of cache      */
static size_t cache_len = 0;   /* number of terms stored         */
static seq_t last_checked = 0; /* largest integer already examined */

static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Return the nth term (n ≥ 1) of A226726.  If n exceeds the length
   of the known sequence the function returns 0. */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold more terms if needed. */
    while (cache_len < (size_t)n) {
        last_checked++;
        seq_t x = last_checked;
        seq_t dp = digit_product(x);
        seq_t half = x / 2;
        idx_t xi = (idx_t)x;
        bool is_even = ((xi & 1ULL) == 0);
        bool ok = false;

        if (is_even) {
            ok = (dp == half);
        } else { /* odd */
            ok = (dp == half) || (dp == half + 1);
        }

        if (ok) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = x;
        }

        /* The sequence is known to be finite; stop if no new terms
           appear up to a generous limit. */
        const seq_t LIMIT = 100000000; /* 1e8 should be enough */
        if (last_checked > LIMIT) break;
    }

    return (cache_len >= (size_t)n) ? cache[n - 1] : (seq_t)0;
}
