/*
 * A044421  Numbers n such that string 8,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose decimal representation
 *            contains * the substring "89" but the integer n-1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

static seq_t *seq_cache = NULL;      /* cached terms of the sequence */
static size_t seq_cache_len = 0;     /* number of cached terms */
static size_t seq_cache_cap = 0;     /* allocated capacity of seq_cache */
static seq_t seq_last_checked = 0;   /* last integer examined */

/* Return true if decimal representation of x contains substring "89". */
static bool contains_89(seq_t x)
{
    static uint8_t buf[128];   /* buffer for decimal digits, LSB first */
    size_t len;

    get_digits(x, buf, &len);   /* buf[0] = LSB, buf[len-1] = MSB */

    for (size_t i = len; i > 1; --i) {
        if (buf[i - 1] == 8 && buf[i - 2] == 9)
            return true;
    }
    return false;
}

/* Ensure seq_cache has capacity at least `need`. */
static void ensure_seq_cache_cap(size_t need)
{
    while (seq_cache_cap < need) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
}

/* Public function: return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Reject n <= 0, but the caller guarantees n >= 1. */
    if (n == 0) return (seq_t)0;

    /* Generate terms until we have at least n cached. */
    while (seq_cache_len < (size_t)n) {
        seq_t candidate = ++seq_last_checked;     /* next integer to test */

        if (!contains_89(candidate))
            continue;                             /* candidate must contain 89 */

        seq_t prev = candidate - 1;
        if (contains_89(prev))
            continue;                             /* previous must not contain 89 */

        /* Store candidate in cache. */
        ensure_seq_cache_cap(seq_cache_len + 1);
        seq_cache[seq_cache_len++] = candidate;
    }

    return seq_cache[n - 1];
}
