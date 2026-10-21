/*
 * A095784  Values of n for which A095777(n) is 15 (those terms which are
 *          expressible in decimal digits for bases 2 through 16, but not for
 *          base 17).
 * https://oeis.org/A095784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   digits_ge10, digits_le9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095784
   a(n) = the nth positive integer that can be written in bases 2–16
          using only decimal digits (0–9) but not in base 17. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digits_ge10.h"
#include "../utils/digits_le9.h"

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n) {
    /* static storage for memoization */
    static seq_t *list = NULL;
    static idx_t list_cap = 0;
    static idx_t terms_found = 0;
    static seq_t last_checked = 1;   /* next number to test */

    /* grow the list storage when needed */
    if (list == NULL) {
        list_cap = 256;
        list = (seq_t *)malloc(list_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (terms_found < n) {
        /* test last_checked for the required property */
        bool ok = true;
        for (int base = 11; base <= 16; ++base) {
            if (!digits_le9(last_checked, base)) {
                ok = false;
                break;
            }
        }
        /* base 17 must fail (i.e., contain a digit >= 10) */
        if (ok && !digits_ge10(last_checked, 17))
            ok = false;

        if (ok) {
            /* store the term */
            if (terms_found == list_cap) {
                list_cap *= 2;
                list = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
            }
            list[terms_found++] = last_checked;
        }
        ++last_checked;
    }

    return list[n - 1];
}
