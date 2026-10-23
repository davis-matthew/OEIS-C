/*
 * A044719  Numbers n such that string 0,6 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computed by brute‑force search for decimal numbers that contain
   the substring “06” but whose successor does not. */

static bool has_06(seq_t x)
{
    bool hasPrev = false;
    int prevDigit = 0;
    const seq_t ten = 10;

    while (x > 0) {
        int d = (int)(x % ten);  /* current least significant digit */
        if (hasPrev && prevDigit == 6 && d == 0) {
            return true;           /* pattern 0‑6 found */
        }
        hasPrev = true;
        prevDigit = d;
        x /= ten;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behavior below the offset is irrelevant */

    static seq_t *cache = NULL;    /* cached terms */
    static size_t cap   = 0;       /* current capacity of cache */
    static size_t sz    = 0;       /* number of cached terms */
    static seq_t next   = 0;       /* next integer to examine */

    while (sz < n) {
        seq_t x = next;
        /* search for the next number satisfying the condition */
        while (!(has_06(x) && !has_06(x + 1))) {
            ++x;
        }

        /* grow cache if needed */
        if (sz == cap) {
            size_t new_cap = cap ? (cap << 1) : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        cache[sz++] = x;
        next = x + 1;  /* start searching after the found term */
    }

    return cache[n - 1];
}
