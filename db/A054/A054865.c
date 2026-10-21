/*
 * A054865  Numbers not divisible by any of their digits when written in base
 *          9.
 * https://oeis.org/A054865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A054865:  Numbers not divisible by any of their digits when written in base 9.
   The algorithm enumerates integers, tests the property by
   extracting base‑9 digits and checking divisibility only for non‑zero digits.
   The sequence is memoised so that repeated calls are fast. */

#include <stdlib.h>   /* realloc, NULL */

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *table = NULL;
    static size_t capacity = 0;
    static size_t length   = 0;   /* number of terms stored */
    static idx_t next_int  = 1;    /* next integer to test */

    /* Grow the table when needed */
    if (length < n) {
        /* Ensure enough capacity for the table */
        if (length == capacity) {
            size_t new_cap = capacity ? capacity << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* allocation failure */
            table = tmp;
            capacity = new_cap;
        }

        /* Search for further terms until we have at least n terms */
        while (length < n) {
            seq_t val = (seq_t)next_int;   /* convert current integer to seq_t */
            seq_t tmp = val;
            int ok = 1;   /* assume valid until a violating digit is found */

            /* Extract base‑9 digits */
            while (tmp > 0) {
                seq_t d = tmp % 9;   /* digit in base 9 */
                if (d != 0) {
                    if (val % d == 0) {   /* divisible by a non‑zero digit */
                        ok = 0;
                        break;
                    }
                }
                tmp /= 9;
            }

            if (ok) {
                table[length++] = val;    /* store the found valid term */
            }
            next_int++;    /* move to the next integer to test */
        }
    }

    /* n is 1‑based; array indices are 0‑based */
    return table[n - 1];
}
