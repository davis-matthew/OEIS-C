/*
 * A039282  Numbers whose base-4 representation has the same nonzero number of
 *          2's and 3's.
 * https://oeis.org/A039282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers with equal non‑zero counts of 2's and 3's in base‑4
 *            representation
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache for the generated sequence terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Next candidate to test for the property */
static seq_t next_candidate = 1;

/* Return true if num has the same positive number of 2's and 3's in its base‑4 digits. */
static bool has_equal_2_3(seq_t num)
{
    unsigned long long cnt2 = 0;
    unsigned long long cnt3 = 0;

    seq_t t = num;
    while (t != 0) {
        seq_t d = t % 4;
        if (d == 2) ++cnt2;
        else if (d == 3) ++cnt3;
        t /= 4;
    }
    return (cnt2 > 0) && (cnt2 == cnt3);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for the OEIS offset, return 0 */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Find the next integer that satisfies the property */
        while (!has_equal_2_3(next_candidate))
            ++next_candidate;

        /* Grow the cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();               /* out of memory */
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Store the found term and advance the candidate pointer */
        cache[cache_len] = next_candidate;
        ++cache_len;
        ++next_candidate;
    }

    return cache[n - 1];
}
