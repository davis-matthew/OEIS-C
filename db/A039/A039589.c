/*
 * A039589  Numbers whose base-6 representation has the same number of 2's,
 *          3's and 4's.
 * https://oeis.org/A039589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer whose base-6 representation *
 *            has an equal number of digits 2, 3, and 4
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple cache to store discovered terms.
 * It examines successive integers starting from 0, keeping those that
 * satisfy the digit‑count equality.  The function is thread‑safe with
 * respect to sequential calls but does not synchronize for concurrent
 * access, which is acceptable for the intended usage. */

#include <stdlib.h>
#include <stdbool.h>

/* Cache state */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* allocated array size */
static idx_t cache_size = 0;      /* number of terms stored in cache */
static idx_t next_term = 0;       /* next integer to test */

/* Ensure the cache has at least 'needed' capacity. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return true if 'val' has equal counts of 2, 3 and 4 in base 6. */
static bool qualifies(seq_t val)
{
    /* Only the digits 2,3,4 are relevant. */
    int cnt2 = 0, cnt3 = 0, cnt4 = 0;

    if (val != 0) {
        seq_t tmp = val;
        while (tmp > 0) {
            int d = (int)(tmp % (seq_t)6);
            if (d == 2)      ++cnt2;
            else if (d == 3) ++cnt3;
            else if (d == 4) ++cnt4;
            tmp /= (seq_t)6;
        }
    }
    /* For val == 0 all counts are zero, which satisfies the equality. */
    return (cnt2 == cnt3 && cnt3 == cnt4);
}

/* Public sequence generator */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least 'n' terms in the cache. */
    while (cache_size < n) {
        seq_t val = (seq_t)next_term;
        if (qualifies(val)) {
            ensure_cache_cap((size_t)cache_size + 1);
            cache[cache_size++] = val;
        }
        ++next_term;
    }
    return cache[n - 1];
}
