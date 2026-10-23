/*
 * A044688  Numbers n such that string 6,2 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_62
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers n such that the base‑9 representation of n
   contains the digit pair 6,2 but the base‑9 representation of n+1
   does not.  The function is memoised so calls can come in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_62.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache of found numbers */
    static seq_t *list = NULL;  /* array of found numbers          */
    static size_t sz   = 0;     /* number of stored terms          */
    static size_t cap  = 0;     /* current capacity                 */
    static seq_t cand   = 0;    /* current candidate under test     */

    /* Generate candidates until we have at least n terms cached. */
    while (sz < n) {
        ++cand;  /* next integer to test */

        if (contains_62(cand) && !contains_62(cand + 1)) {
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* allocation failed */
                list = tmp;
                cap  = new_cap;
            }
            list[sz++] = cand;
        }
    }

    return list[n - 1];
}
