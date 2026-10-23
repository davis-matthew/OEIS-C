/*
 * A044155  Numbers k such that string 2,3 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k such that the string "23" occurs in
 *            the base‑7 representation of k but does not occur in the
 *            representation of k−1
 * Range:     u8 n <= 6, u16 n <= 1446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static inline bool contains23(seq_t x)
{
    if (x == (seq_t)0)
        return false;
    seq_t prev = (seq_t)-1;            /* sentinel that is never 3 */
    while (x != (seq_t)0) {
        unsigned digit = (unsigned)(x % (seq_t)7);
        if (digit == 2 && prev == 3)
            return true;
        prev = digit;
        x /= (seq_t)7;
    }
    return false;
}

/* dynamic cache of found terms */
static seq_t *cache = NULL;
static size_t cache_size   = 0;        /* number of terms stored */
static size_t cache_cap    = 0;        /* allocated capacity */
static idx_t  max_checked  = 0;        /* last integer examined */
static bool   last_has23   = false;    /* contains23(max_checked) */

/* grow cache to at least min_cap entries */
static void ensure_capacity(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < min_cap)
            new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n<1 is undefined */
        return (seq_t)0;

    /* already have enough terms cached? */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* generate terms until the nth one is found */
    seq_t current = max_checked + (seq_t)1;   /* next integer to test */

    while (cache_size < (size_t)n) {
        bool cur_has23 = contains23(current);

        if (cur_has23 && !last_has23) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = current;
        }

        last_has23 = cur_has23;
        max_checked++;
        current++;
    }

    return cache[n - 1];
}
