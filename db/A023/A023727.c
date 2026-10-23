/*
 * A023727  Numbers with exactly 2 1's in their base 5 expansion.
 * https://oeis.org/A023727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_ones_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerate successive integers, counting the digits equal to 1 in
 * base‑5; keep only those with exactly two such digits.  Cache the results
 * so that repeated calls with arbitrary n return fast.
 * This implements OEIS sequence A023727: Numbers with exactly two 1's in their
 * base‑5 expansion. */

/* required standard header */
#include <stdlib.h>
#include "../utils/count_ones_base5.h"

/* static cache for memoisation */
#define MAX_CACHE 20000                 /* enough for n up to about 10000 */
static seq_t cache[MAX_CACHE];
static size_t cache_sz = 0;
static seq_t last_val = 0;

/* ensure that cache contains at least n terms (n >= 1) */
static void ensure_cache(idx_t n) {
    while (cache_sz < n && cache_sz < MAX_CACHE) {
        seq_t val = last_val;
        ++last_val;                     /* advance to next integer */

        if (count_ones_base5(val) == 2)
            cache[cache_sz++] = val;
    }
    /* If n exceeds MAX_CACHE we simply ignore the request; the problem
       constraints guarantee n <= about 10000. */
}

/* public entry point required by the build system */
seq_t a(idx_t n) {
    if (n == 0)                          /* offset is 1; below offset undefined */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
