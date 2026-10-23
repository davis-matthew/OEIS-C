/*
 * A043875  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 8 mod 10.
 * https://oeis.org/A043875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   a043875_runs_in_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A043875 – Numbers k such that the number of runs in the base‑4
 * representation of k is congruent to 8 modulo 10.
 *
 * a(n) returns the n‑th such number (1‑based).  The function builds a
 * small static cache of the found terms and extends it only until it
 * contains the requested index.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a043875_runs_in_base4.h"

#ifndef MAX_A043875_TERMS
/* enough for n up to about 10000 (density ≈ 1/10 of all integers). */
#  define MAX_A043875_TERMS 20000
#endif

static seq_t  a043875_cache[MAX_A043875_TERMS];
static idx_t  a043875_cache_len = 0;
static seq_t  a043875_last_checked = 0;

/* Return the n‑th term of A043875 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Expand cache until it contains at least n terms. */
    while (a043875_cache_len < n) {
        seq_t cand = a043875_last_checked + 1;
        a043875_last_checked = cand;

        if (a043875_runs_in_base4(cand) % 10 == 8) {
            if (a043875_cache_len < MAX_A043875_TERMS) {
                a043875_cache[a043875_cache_len++] = cand;
            } else {
                /* Should not happen for the problem limits. */
                /* If needed, wrap-around or abort would be appropriate. */
            }
        }
    }
    return a043875_cache[n - 1];
}
