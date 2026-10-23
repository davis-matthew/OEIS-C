/*
 * A022514  Describe previous term from the right (method B - initial term is
 *          3).
 * https://oeis.org/A022514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = look‑and‑say read from right, grouping equal digits,
 *            digit then frequency. a(0) = 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, grow_seq_array, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/grow_seq_array.h"

/* Compute next term from the current one. */
static seq_t next_term(seq_t term)
{
    seq_t result = 0;
    seq_t tmp = term;

    while (tmp > 0) {
        seq_t digit = tmp % 10;
        seq_t cnt   = 0;

        /* count consecutive identical digits from the right */
        while (tmp > 0 && (tmp % 10) == digit) {
            cnt++;
            tmp /= 10;
        }

        /* length of the count in decimal digits */
        int lencnt = digit_len(cnt);
        seq_t pow10len   = pow10((idx_t)lencnt);
        seq_t piece      = digit * pow10len + cnt;          /* digit followed by count */
        int  piece_len   = 1 + lencnt;                      /* decimal digits in piece */
        seq_t pow10piece = pow10((idx_t)piece_len);

        result = result * pow10piece + piece;
    }
    return result;
}

/* Return the n‑th term of the sequence.  Calls may be in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_len = 0;     /* number of valid terms stored */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        cache_len = 0;
        cache_cap = 0;
    }

    size_t target = (size_t)n + 1;    /* we need terms up to index n inclusive */

    if (cache_len < target) {
        /* enlarge cache to accommodate the required index */
        grow_seq_array(&cache, &cache_cap, target);

        /* compute missing terms */
        for (size_t i = cache_len; i < target; ++i) {
            if (i == 0) {
                cache[0] = 3;          /* initial term */
            } else {
                cache[i] = next_term(cache[i - 1]);
            }
        }
        cache_len = target;
    }

    return cache[(size_t)n];
}
