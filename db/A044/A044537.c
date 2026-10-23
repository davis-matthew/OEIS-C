/*
 * A044537  Numbers n such that string 2,4 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Sequence A044537 – Numbers n such that the base‑7 representation of n
   contains the substring "24" but the base‑7 representation of n+1 does not.
   The function a(n) returns the n‑th such number (n ≥ 1).
   This implementation performs an incremental search with memoisation
   to avoid re‑computing the sequence for repeated or non‑monotonic calls.
*/

#include <stdlib.h>
#include <stdbool.h>

/* Helper: return true if the base‑7 representation of x contains the
   substring 2→4 (most significant digit 2 followed by 4). */
static bool has24(seq_t x)
{
    /* base‑7 digits are always ≤6 => uint8_t is enough */
    uint8_t digits[64];         /* enough for any 64‑bit number (and beyond) */
    idx_t len = 0;

    /* extract digits LSB → MSB */
    while (x > 0) {
        seq_t d = x % 7;
        digits[len++] = (uint8_t)d;
        x = x / 7;
    }

    if (len < 2)
        return false;

    /* scan from MSB to LSB for pattern 2,4 */
    for (idx_t i = len - 1; i > 0; --i) {
        if (digits[i] == 2 && digits[i - 1] == 4)
            return true;
    }
    return false;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache       = NULL;
    static idx_t cache_cap    = 0;   /* allocated array size */
    static idx_t max_cached   = 0;   /* number of terms stored */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;
    }

    /* grow cache if required for this call */
    if (n > cache_cap) {
        idx_t new_cap = n * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* return already known result */
    if (n <= max_cached)
        return cache[n - 1];

    /* start searching from the next integer after the last cached term,
       or from 18 if nothing is cached yet. */
    seq_t current = (max_cached == 0) ? (seq_t)18 : cache[max_cached - 1] + 1;
    idx_t  count  = max_cached;

    while (count < n) {
        if (has24(current) && !has24(current + 1)) {
            cache[count++] = current;     /* store new term */
        }
        current++;                          /* move to next integer */
    }

    max_cached = n;                         /* update cache size */
    return cache[n - 1];
}
