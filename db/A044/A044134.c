/*
 * A044134  Numbers n such that string 5,2 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer n such that the base‑6 representation
 *            of n contains the consecutive digits 5,2 while the base‑6
 *            representation of n-1 does not
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the implementation searches integers sequentially and caches all
   found terms.  The cache is expanded with realloc; the cast is added
   to keep the code valid in both C and C++. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether the base‑6 representation of x contains ...52 ... */
static bool contains52(seq_t x)
{
    if (x == 0)
        return false;

    seq_t last = 0;
    bool first = true;
    while (x > 0) {
        seq_t d = x % 6;
        if (!first && d == 5 && last == 2)
            return true;
        last = d;
        first = false;
        x = x / 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  Undefined for n == 0 but handled gracefully. */
    if (n == 0)
        return 0;

    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t len = 0, cap = 0;
    /* next integer to examine */
    static seq_t next = 1;

    /* generate terms until we have n of them */
    while (len < n) {
        seq_t cur = next;
        bool curHas   = contains52(cur);
        bool prevHas  = (cur > 0) ? contains52(cur - 1) : false;

        if (curHas && !prevHas) {
            if (len == cap) {
                size_t new_cap = (cap > 0) ? cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                terms = tmp;
                cap = new_cap;
            }
            terms[len++] = cur;
        }
        ++next;
    }
    return terms[n - 1];
}
