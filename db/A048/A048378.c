/*
 * A048378  Divisible by the concatenation of its first digit-differences
 *          (leading zeros allowed).
 * https://oeis.org/A048378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 337, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A048378 by brute‑force search.
   a(1) = 10, a(2) = 12, a(3) = 20, … as given in OEIS.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether x satisfies the property
   (x divisible by the concatenation of the absolute differences
   of consecutive decimal digits, allowing leading zeros).  */
static bool satisfies(uint64_t x)
{
    if (x < 10ULL)          /* must have at least two digits */
        return false;

    /* extract digits into an array (least significant first) */
    uint8_t digs[20];      /* enough for 64‑bit numbers */
    size_t d = 0;
    uint64_t tmp = x;
    while (tmp) {
        digs[d++] = (uint8_t)(tmp % 10ULL);
        tmp /= 10ULL;
    }

    /* concatenate the absolute differences of adjacent digits */
    uint64_t c = 0;
    for (size_t i = d; i > 1; --i) {
        uint8_t diff = (digs[i-1] > digs[i-2]) ? digs[i-1] - digs[i-2]
                                                : digs[i-2] - digs[i-1];
        c = c * 10ULL + diff;
    }
    if (c == 0ULL)
        return false;   /* division by zero would occur */

    return (x % c == 0ULL);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* behaviour below offset is irrelevant */

    /* static cache of already found terms */
    /* capacity chosen large enough for typical tests (12 k terms ~ 100 0000
       terms would use <5 MB on 64‑bit).  */
    #define CACHE_CAP 250000
    static seq_t cache[CACHE_CAP];
    static size_t len  = 0;          /* number of terms stored */
    static uint64_t last_checked = 1; /* next integer to test */

    /* grow cache until it contains at least n elements */
    while (len < (size_t)n) {
        if (satisfies(last_checked)) {
            if (len < CACHE_CAP) {
                cache[len++] = (seq_t)last_checked;
            } else {
                /* capacity exceeded – should not happen for n <= 10 000 */
                abort();
            }
        }
        ++last_checked;
    }

    return cache[(size_t)n - 1];
}
