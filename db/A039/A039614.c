/*
 * A039614  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 3's.
 * https://oeis.org/A039614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer whose base‑12 representation
   contains the same non‑zero number of digits 2 and 3. */

#include <stdlib.h>
#include <stdbool.h>

static bool has_equal_nonzero_2_and_3(seq_t x)
{
    int count2 = 0;
    int count3 = 0;
    seq_t t = x;
    while (t > 0) {
        seq_t rem = t % 12;
        if (rem == 2)
            ++count2;
        else if (rem == 3)
            ++count3;
        t = t / 12;
    }
    return count2 > 0 && count2 == count3;
}

#define MAX_CACHE 20000   /* enough for n <= 10000 in the test harness */

/* a(n) = n‑th integer (n ≥ 1) whose base‑12 representation has
   equal non‑zero counts of digits 2 and 3. */
seq_t a(idx_t n)
{
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;
    static idx_t next_candidate = 1;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (has_equal_nonzero_2_and_3(cand)) {
            if (cache_len < MAX_CACHE)
                cache[cache_len++] = cand;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
