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
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095784
   a(n) = the nth positive integer that can be written in bases 2–16
          using only decimal digits (0–9) but not in base 17. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: does x in the given base contain at least one digit >= 10? */
static bool digits_ge10(seq_t x, int base) {
    seq_t y = x;
    while (y > 0) {
        if ((y % base) >= 10)
            return true;
        y /= base;
    }
    return false;
}

/* Helper: are all digits of x in the given base <= 9? */
static bool digits_le9(seq_t x, int base) {
    seq_t y = x;
    while (y > 0) {
        if ((y % base) >= 10)
            return false;
        y /= base;
    }
    return true;
}

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
        list = mem_new(seq_t, list_cap);
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
                list = mem_resize(seq_t, list, list_cap);
            }
            list[terms_found++] = last_checked;
        }
        ++last_checked;
    }

    return list[n - 1];
}
