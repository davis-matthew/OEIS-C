/*
 * A026362  a(n) = a(m) if a(m) has already occurred exactly once and n =
 *          a(m)+2m-1, else a(n) = least positive integer that has not yet
 *          occurred.
 * https://oeis.org/A026362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   extend_freq_array, extend_seq_t_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(m) if a(m) has already appeared exactly once
   and n = a(m)+2m-1; otherwise a(n) = least positive integer
   not yet seen.  This implementation memoises the sequence and
   works for calls in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/extend_freq_array.h"
#include "../utils/extend_seq_t_array.h"

/* The sequence function */
seq_t a(idx_t n)
{
    /* Off‑by‑one: a(1) is the first term. */
    if (n < 1) return 0;   /* undefined below offset */

    /* Static memoised state ------------------------------------------- */
    static seq_t      *seq      = NULL;   /* sequence values, 1‑based */
    static size_t       seq_cap  = 0, seq_len  = 0;
    static seq_t      *reuse    = NULL;   /* value scheduled for a future index, 1‑based */
    static size_t       reuse_cap = 0;
    static unsigned char *freq   = NULL;   /* occurrence counts, 1‑based */
    static size_t       freq_cap   = 0;
    static unsigned long next_candidate = 1;  /* smallest unused integer */

    /* Grow the cached sequence up to index n ------------------------- */
    for (idx_t i = seq_len + 1; i <= n; ++i)
    {
        /* Ensure capacity of the three arrays */
        extend_seq_t_array(&seq,   &seq_cap, (size_t)i);
        extend_seq_t_array(&reuse, &reuse_cap, (size_t)i);

        seq_t val;
        bool first_occurrence = false;

        /* If a value is scheduled for this index, use it */
        if (reuse[i] != 0) {
            val = reuse[i];
        } else {
            /* Find the smallest positive integer not yet seen */
            while (true) {
                size_t cand = (size_t)next_candidate;
                extend_freq_array(&freq, &freq_cap, cand);
                if (freq[cand] == 0) break;
                ++next_candidate;
            }
            val = next_candidate;
        }

        /* Update frequency information */
        size_t idx = (size_t)val;
        extend_freq_array(&freq, &freq_cap, idx);
        if (freq[idx] == 0) first_occurrence = true;
        ++freq[idx];

        /* Assign the computed value */
        seq[i] = val;
        ++seq_len;

        /* Schedule reuse for first occurrences */
        if (first_occurrence) {
            /* target = a(m) + 2m - 1 where m = i and a(m) = val */
            __int128 target128 = (__int128)val + 2 * (__int128)i - 1;
            /* The target index must fit in idx_t for the problem limits. */
            idx_t target = (idx_t)target128;
            extend_seq_t_array(&reuse, &reuse_cap, (size_t)target);
            if (reuse[target] == 0)  /* avoid accidental overwrite */
                reuse[target] = val;
        }
    }

    return seq[n];
}
