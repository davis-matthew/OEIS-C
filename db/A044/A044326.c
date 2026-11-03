/*
 * A044326  Numbers n such that string 8,3 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044326
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

/* Search-based implementation for OEIS A044326:
   the n‑th integer whose base‑9 representation contains the
   substring 8,3 but whose predecessor does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x in base 9 contains the substring 8,3. */
static bool has83(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;                 /* previous (less‑significant) digit */
    while (x > 0) {
        int d = (int)(x % 9);      /* current more‑significant digit */
        if (d == 8 && prev == 3)  /* pattern 8 (more), 3 (less) */
            return true;
        prev = d;
        x = x / 9;
    }
    return false;
}

/* Return the n-th term of A044326 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */

    /* Static cache for already computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* number of terms stored */
    static size_t terms_cap = 0;   /* allocated capacity       */
    static seq_t next_num = 1;     /* next candidate number    */

    if (n > terms_len) {           /* need more terms           */
        if (terms == NULL) {       /* first allocation          */
            terms_cap = 128;
            terms = mem_new(seq_t, terms_cap);
        }
        while (terms_len < n) {    /* compute until we have n   */
            seq_t num = next_num;
            if (has83(num) && !has83(num - 1)) {
                if (terms_len == terms_cap) { /* grow buffer    */
                    terms_cap <<= 1;
                    terms = mem_resize(seq_t, terms, terms_cap);
                }
                terms[terms_len++] = num;
            }
            next_num++;
        }
    }
    return terms[n - 1];
}
