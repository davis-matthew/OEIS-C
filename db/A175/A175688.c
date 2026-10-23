/*
 * A175688  Numbers k with property that arithmetic mean of its digits is both
 *          an integer and one of the digits of k.
 * https://oeis.org/A175688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 6854, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175688: numbers k whose decimal digit mean is an integer appearing among its digits.
   Computed by incremental search. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number satisfies the property. */
static bool _is_good(seq_t x)
{
    /* The number 0 is a special case: its mean is 0, which is a digit of 0. */
    if (x == 0)
        return true;

    seq_t tmp = x;
    unsigned sum   = 0;
    unsigned cnt   = 0;
    unsigned mask  = 0;   /* bit mask of digits that appear */

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        tmp /= 10;
        sum  += d;
        cnt  += 1;
        mask |= 1u << d;
    }

    if (sum % cnt != 0)
        return false;

    unsigned avg = sum / cnt;
    if (avg > 9)          /* average cannot be a digit if >9 */
        return false;

    return (mask & (1u << avg)) != 0;
}

/* Retrieve the n-th term (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of already generated terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static idx_t next   = 0;   /* next candidate to test */

    /* Grow the cache until it contains at least n terms. */
    while (size < (size_t)n) {
        seq_t cand = (seq_t)next;
        if (_is_good(cand)) {
            if (size >= cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmpbuf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmpbuf;
                cap   = new_cap;
            }
            cache[size++] = cand;
        }
        next++;
    }

    return cache[(size_t)n - 1];
}
