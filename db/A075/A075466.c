/*
 * A075466  Trajectory of 266718 under the Reverse and Add! operation carried
 *          out in base 4, written in base 10.
 * https://oeis.org/A075466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075466: Trajectory of 266718 under Reverse and Add in base 4.
   a(0) = 266718
   a(n+1) = a(n) + reverse_base4(a(n))
   reverse_base4(x) = integer obtained by reversing the base‑4 digits of x */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Reverse the base‑4 representation of x.  The function works
   for all supported seq_t types (fixed‑size unsigned integers
   or arbitrary‑precision C++ classes). */
static seq_t reverse_base4(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t d = x % 4;          /* current least‑significant base‑4 digit */
        rev = rev * 4 + d;       /* append it to the reversed number */
        x /= 4;                  /* move to next digit */
    }
    return rev;
}

/* a(n) : nth term of the sequence.  The function uses a
   lazy, growing cache so that calls in arbitrary order are
   O(1) after the first computation of each index.  */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;   /* dynamic array of computed terms */
    static size_t cap   = 0;      /* capacity in number of elements */
    static idx_t  last  = -1;     /* highest computed index (initially none) */

    /* initialize cache on first call */
    if (cache == NULL) {
        grow_seq(&cache, &cap, 1);   /* allocate space for at least one element */
        cache[0] = 266718;          /* a(0) */
        last = 0;
    }

    /* compute missing values up to n */
    while (last < n) {
        /* ensure capacity for next element */
        if ( (size_t)(last + 1) >= cap ) {
            grow_seq(&cache, &cap, (size_t)(last + 2));
        }
        cache[last + 1] = cache[last] + reverse_base4(cache[last]);
        ++last;
    }

    return cache[n];
}
