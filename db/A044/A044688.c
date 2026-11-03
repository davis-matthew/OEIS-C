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
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers n such that the base‑9 representation of n
   contains the digit pair 6,2 but the base‑9 representation of n+1
   does not.  The function is memoised so calls can come in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if base‑9 representation of x has an adjacent digit pair
   6 followed by 2 (more significant digit 6, next lower digit 2). */
static bool contains_62(seq_t x)
{
    /* A 128‑bit number needs at most about 100 base‑9 digits; 200
       is safely enough for all supported arbitrary‑precision types. */
    unsigned char digits[200];
    size_t len = 0;

    if (x < 2)  /* cannot contain 62 if less than 2 digits in base‑9 */
        return false;

    while (x > 0) {
        digits[len++] = (unsigned char)(x % 9);
        x /= 9;
    }

    /* Scan from least significant to most significant digit.
       Looking for pattern: higher digit 6, next lower digit 2. */
    for (size_t j = 1; j < len; ++j) {
        if (digits[j] == 6 && digits[j - 1] == 2)
            return true;
    }
    return false;
}

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
                seq_t *tmp = mem_resize(seq_t, list, new_cap);
                if (!tmp) exit(1);   /* allocation failed */
                list = tmp;
                cap  = new_cap;
            }
            list[sz++] = cand;
        }
    }

    return list[n - 1];
}
