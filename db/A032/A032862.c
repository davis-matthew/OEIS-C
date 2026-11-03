/*
 * A032862  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(m)
 *          > d(m-1) < d(m-2) > ...
 * https://oeis.org/A032862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer (1‑based) whose base‑7 digits alternate
 *            as d(m) > d(m-1) < d(m-2) > d(m-3) < … (starting with “>” at the
 *            most significant two digits). The sequence starts
 *            0,1,2,3,4,5,6,7,14…
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether a number satisfies the alternating (> < …) pattern in base 7.
   The number x may be of any build‑dependent integer type seq_t. */
static bool good_base7(seq_t x)
{
    seq_t t = x;
    if (t == 0)               /* single digit 0 is acceptable */
        return true;

    /* 256 digits is far more than enough for any 64‑bit integer in base 7. */
    uint8_t digits[256];
    int len = 0;
    while (t != 0) {
        digits[len++] = (uint8_t)(t % 7);
        t /= 7;
    }

    bool greater = true;      /* first comparison must be “>” */
    for (int i = len - 1; i > 0; --i) {
        if (greater) {
            if (digits[i] <= digits[i - 1])
                return false;
        } else {
            if (digits[i] >= digits[i - 1])
                return false;
        }
        greater = !greater;
    }
    return true;
}

/* Static cache of previously computed terms. */
static seq_t *cache       = NULL;   /* array of sequence values */
static size_t cache_size  = 0;      /* number of terms stored so far   */
static size_t cache_cap   = 0;      /* allocated capacity              */
static idx_t next_number  = 0;      /* candidate for next test         */

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n values. */
    while (cache_size < (size_t)n) {
        if (good_base7(next_number)) {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap) {
                size_t newcap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) abort();       /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = (seq_t)next_number;
        }
        ++next_number;
    }
    return cache[n - 1];
}
