/*
 * A044613  Numbers n such that string 5,5 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044613:
   a(n) is the nth number n such that the base‑8 representation of n
   contains the substring "55" while the base‑8 representation of n+1
   does not.  The sequence starts with a(1)=45, a(2)=109, … */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether the base‑8 expansion of x contains a consecutive pair of 5s. */
static bool has55(seq_t x)
{
    int prev = -1;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)8);        /* current least‑significant digit */
        if (prev == 5 && d == 5) return true;
        prev = d;
        x = x / (seq_t)8;
    }
    return false;
}

/* Return the nth term (1‑based).  Any call order is supported. */
seq_t a(idx_t n)
{
    static seq_t *list = NULL;   /* cache of found terms */
    static size_t len   = 0;     /* number of terms cached */
    static size_t cap   = 0;     /* capacity of the cache */
    static seq_t cur;           /* next integer to test */

    if (n == 0) return (seq_t)0;          /* undefined for 0, return 0 */

    /* Initialise on first call */
    if (len == 0) {
        cur = (seq_t)1;
        cap = 16;
        list = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        seq_t val = cur;

        if (has55(val) && !has55(val + (seq_t)1)) {
            if (len == cap) {            /* grow the cache */
                cap *= 2;
                list = mem_resize(seq_t, list, cap);
            }
            list[len++] = val;
        }
        cur = cur + (seq_t)1;
    }

    return list[n - 1];
}
