/*
 * A044583  Numbers n such that string 1,7 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044583
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

/*  Sequence A044583: numbers n such that base‑8 representation of n contains
    the substring "17" but base‑8 representation of n+1 does not.  */

/* Implements a(idx_t n) return the n‑th such integer.  The function
   searches forward and memoises results, so it works correctly for any
   order of calls.  It is suitable for seq_t being an unsigned integer
   type (8, 16, 32, 64, 128 bits) or an arbitrary‑precision integer class. */

/*  The routine may be used in C11 and C++17.  */
#include <stdlib.h>
#include <stdbool.h>

/* Helper: does x have the consecutive digits 1,7 in its base‑8 representation? */
static bool has17(seq_t x)
{
    if (x == 0) return false;

    /* 128‑bit numbers need at most 43 octal digits, 64 is enough. */
    uint8_t digits[64];
    size_t d = 0;
    seq_t y = x;
    while (y > 0) {
        digits[d++] = (uint8_t)(y % (seq_t)8);
        y /= (seq_t)8;
    }

    /* Scan from most significant to least. */
    for (size_t i = d; i-- > 1; ) {
        if (digits[i] == 1 && digits[i-1] == 7) return true;
    }
    return false;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t last_checked = 0;

    /* grow cache if needed */
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* extend cache until we have n terms */
    while (terms_len < (size_t)n) {
        last_checked += 1;
        if (has17(last_checked) && !has17(last_checked + 1))
            terms[terms_len++] = last_checked;
    }

    return terms[n - 1];
}
