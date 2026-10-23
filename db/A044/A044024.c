/*
 * A044024  Numbers k such that 3 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) are numbers k such that the decimal representation of k
 * contains the adjacent digits 3 and 8 in either order (i.e., substrings
 * "38" or "83"), while the decimal representation of k+1 does not
 * contain such adjacent digits.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static seq_t  next_candidate = 38;   /* first number to test */

/* Return true if x has the adjacent digits 3 and 8 in either order. */
static bool has_adjacent_38_or_83(seq_t x)
{
    if (x < 10)               /* single‑digit numbers cannot contain it */
        return false;

    seq_t prev = x % 10;
    x /= 10;

    while (x > 0) {
        seq_t cur = x % 10;
        if ((prev == 3 && cur == 8) || (prev == 8 && cur == 3))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, n==0 is unused */
        return 0;

    if (n <= seq_len)
        return seq_cache[n - 1];

    /* Generate terms until we have the nth one. */
    while (seq_len < n) {
        seq_t k = next_candidate;

        if (has_adjacent_38_or_83(k) && !has_adjacent_38_or_83(k + 1)) {
            ensure_cache_cap(seq_len + 1);
            seq_cache[seq_len++] = k;
        }

        next_candidate = k + 1;
    }

    return seq_cache[n - 1];
}
