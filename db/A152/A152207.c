/*
 * A152207  Numbers k such that (sum of base-2 digits of k) = (sum of base-10
 *          digits of k) = 10.
 * https://oeis.org/A152207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, binary_digit_sum, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k satisfying sum of binary digits = sum of decimal digits = 10. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"
#include "../utils/binary_digit_sum.h"

/* --- Static cache for the sequence values ----------------------- */
static seq_t *cache   = NULL;   /* dynamic array of found terms */
static size_t cache_len = 0;    /* number of elements currently stored */
static size_t cache_cap = 0;    /* allocated capacity of cache array */
static seq_t last_checked = 0;  /* last number tested for the property */

/* ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but safe for caller */

    /* generate values until we have at least n terms */
    while ((idx_t)cache_len < n) {
        last_checked += 1;
        seq_t val = last_checked;

        /* decimal digit sum */
        if (digit_sum(val) == (seq_t)10) {
            /* binary digit sum */
            if (binary_digit_sum(val) == (seq_t)10) {
                append_candidate(&cache, &cache_len, &cache_cap, val);
            }
        }
    }

    return cache[n - 1];
}
