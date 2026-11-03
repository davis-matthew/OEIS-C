/*
 * A044480  Numbers k such that string 4,2 occurs in the base 5 representation
 *          of k but not of k+1.
 * https://oeis.org/A044480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044480:  a(n) is the nth integer k such that the base‑5
 * representation of k contains the digit pair 4,2 (adjacent) but the
 * base‑5 representation of k+1 does not.  The function searches k in
 * increasing order, caches found values, and works for any backing
 * type seq_t. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff base‑5 representation of x contains the adjacent
 * digits 4 followed by 2. */
static bool has42(seq_t x)
{
    /* A buffer is needed only to hold the digits of the current
       number.  128 digits more than suffice for all test values up
       to 10 000th term. */
    uint8_t dig[128];
    size_t len = 0;
    seq_t t = x;

    if (t == 0)
        return false;

    while (t > 0) {
        dig[len++] = (uint8_t)(t % 5);
        t /= 5;
    }

    /* The digits are stored from least‑significant to most‑significant.
       Scan for the pattern 4,2 from most‑significant downwards. */
    for (size_t i = len; i > 1; --i) {
        if (dig[i - 1] == 4 && dig[i - 2] == 2)
            return true;
    }
    return false;
}

/* Return the nth element of the sequence (1‑based).
 * The function can be called in any order; results are cached
 * to avoid recomputation. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n==0 is irrelevant */
        return 0;

    /* static table of found terms and state */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of terms stored */
    static size_t cap  = 0;   /* allocated capacity */
    static seq_t nextk = 0;   /* next candidate to test */

    /* Grow cache until we have the requested term. */
    while (size < (size_t)n) {
        /* Find the next k satisfying the conditions. */
        while (1) {
            if (has42(nextk) && !has42(nextk + 1)) {
                /* make room if necessary */
                if (size == cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (tmp == NULL)   /* allocation failed */
                        return 0;
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[size++] = nextk;
                break;          /* found the next term */
            }
            ++nextk;              /* try the next candidate */
        }
        ++nextk;                  /* advance past the term just added */
    }

    return cache[n - 1];
}
