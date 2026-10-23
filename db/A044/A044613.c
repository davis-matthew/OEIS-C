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
 * Helpers:   has55
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044613:
   a(n) is the nth number n such that the base‑8 representation of n
   contains the substring "55" while the base‑8 representation of n+1
   does not.  The sequence starts with a(1)=45, a(2)=109, … */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has55.h"

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
        list = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        seq_t val = cur;

        if (has55(val) && !has55(val + (seq_t)1)) {
            if (len == cap) {            /* grow the cache */
                cap *= 2;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
            }
            list[len++] = val;
        }
        cur = cur + (seq_t)1;
    }

    return list[n - 1];
}
