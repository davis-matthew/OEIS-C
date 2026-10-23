/*
 * A039221  Numbers whose base-12 representation has the same number of 1's
 *          and 3's.
 * https://oeis.org/A039221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039221: numbers whose base‑12 representation has the same number of 1’s and 3’s. */
#include <stddef.h>
#include <stdint.h>

/* Helper: test whether a seq_t number has equal # of 1's and 3's in base 12. */
static int equal13(seq_t x)
{
    unsigned ones  = 0;
    unsigned threes = 0;
    while (x != 0) {
        int digit = (int)(x % (seq_t)12);
        if (digit == 1) ++ones;
        else if (digit == 3) ++threes;
        x /= (seq_t)12;
    }
    return ones == threes;
}

/* Cache of sequence values. */
static seq_t *cache  = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t cur = 0;  /* current integer being tested */

/* Ensure cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;  /* abort on allocation failure */
    }
    /* retain existing cache entries */
    while (cache_size < (size_t)n) {
        /* test current number */
        if (equal13(cur)) {
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) return;  /* abort on allocation failure */
            }
            cache[cache_size++] = cur;
        }
        /* move to next number */
        cur += 1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n == 0, but safe */
    ensure_cache(n);
    return cache[n-1];
}
