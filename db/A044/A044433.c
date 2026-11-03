/*
 * A044433  Numbers n such that string 0,0 occurs in the base 3 representation
 *          of n but not of n+1.
 * https://oeis.org/A044433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS A044433: the nth integer n
   whose base‑3 representation contains the substring "00",
   while the base‑3 representation of n+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a base‑3 number contains two consecutive zeros. */
static bool has00(seq_t x)
{
    int consec = 0;
    while (x > 0) {
        seq_t d = x % 3;
        if (d == 0) {
            ++consec;
            if (consec >= 2)
                return true;
        } else {
            consec = 0;
        }
        x /= 3;
    }
    return false;
}

/* Return the nth term (1‑based) of A044433.  Calls may come in any
   order; the result is memoised in a static dynamic array. */
seq_t a(idx_t n)
{
    static seq_t *table = NULL;
    static size_t sz    = 0;
    static size_t cap   = 0;
    static seq_t next   = 1;   /* next integer to test */

    while (sz < n) {
        seq_t m = next;

        if (has00(m) && !has00(m + 1)) {
            /* store m */
            if (sz == cap) {
                size_t newcap = (cap == 0) ? 4 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, table, newcap);
                if (!tmp) abort();   /* memory allocation failure */
                table = tmp;
                cap   = newcap;
            }
            table[sz++] = m;
        }

        ++next;
    }
    return table[n - 1];
}
