/*
 * A043252  Numbers k such that 5 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm: find integers k such that digits 57 or 75 appear
 * in the decimal representation of k but not in k-1.  The sequence is
 * non‑decreasing and is generated on demand; previously computed terms
 * are cached for efficiency. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if x contains the adjacent pair 5–7 or 7–5. */
static bool has_pair_57_or_75(seq_t x)
{
    if (x < 10) return false;          /* single digit cannot contain a pair        */
    seq_t prev = x % 10;                /* lowest digit                                 */
    x /= 10;
    while (x > 0) {
        seq_t cur = x % 10;            /* next digit from the right                   */
        if ((cur == 5 && prev == 7) || (cur == 7 && prev == 5))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not defined for n == 0, defensive return */

    static seq_t *cache = NULL;         /* cached sequence values                      */
    static idx_t  count  = 0;           /* number of values already cached              */
    static idx_t  cap    = 0;           /* current capacity of the cache                */
    static idx_t  scanned_upto = 0;     /* last integer checked (in decimal)            */

    /* Expand the cache if necessary. */
    if (count >= cap) {
        size_t new_cap = cap == 0 ? 64 : cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cap = new_cap;
    }

    /* Search forward until we have at least n terms cached. */
    while (count < n) {
        scanned_upto++;                         /* candidate value k                        */
        seq_t k_val = (seq_t)scanned_upto;      /* cast to seq_t (built‑in integer or big int) */

        if (has_pair_57_or_75(k_val)) {
            /* Check that k-1 does NOT contain the pair. */
            seq_t prev_val = k_val - 1;          /* k-1 is guaranteed >= 0 */
            if (!has_pair_57_or_75(prev_val)) {
                cache[count++] = k_val;          /* store found term                   */
            }
        }
    }

    return cache[n - 1];
}
