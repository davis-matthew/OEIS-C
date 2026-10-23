/*
 * A180061  Numbers k such that (2^(2^k mod k) mod k) = 4.
 * https://oeis.org/A180061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers k such that (2^(2^k mod k) mod k) = 4
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm scans k starting from 1, keeps all k that satisfy the
    condition in a static cache, and returns the n‑th term from that cache. */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t */
#include "../utils/mod_pow.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    /* Persistent cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored          */
    static size_t cache_cap = 0;  /* current allocated capacity       */
    static idx_t last_checked = 0;/* largest k already tested          */

    if (n == 0) return (seq_t)0;  /* safety, though OEIS offset is 1 */

    /* If we already have the requested term, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate more terms until we have at least n */
    for (idx_t k = last_checked + 1;; ++k) {
        /* Compute e = 2^k mod k  */
        seq_t e = mod_pow((seq_t)2, k, (seq_t)k);

        /* e < k, safe to cast to 64‑bit integer for exponent */
        uint64_t exp = (uint64_t)e;

        /* Compute value = 2^e mod k  */
        seq_t val = mod_pow((seq_t)2, exp, (seq_t)k);

        if (val == (seq_t)4)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)k);

        last_checked = k;

        if (cache_len >= n)
            break;
    }

    return cache[n - 1];
}
