/*
 * A055048  Numbers of the form 9^i*(3*j+2).
 * https://oeis.org/A055048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer of the form 9^i*(3*j+2) (i,j ≥ 0)
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_in_form
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_in_form.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1; values for n == 0 are not used. */
    if (n == 0) return 0;

    static seq_t *seq = NULL;
    static size_t len = 0, cap = 0;
    static seq_t next_candidate = 2;   /* next integer to test */

    /* Grow and fill the sequence until we have at least n elements. */
    while (len < n) {
        /* Skip numbers that cannot be in the form. */
        if (is_in_form(next_candidate)) {
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            seq[len++] = next_candidate;
        }
        next_candidate++;
    }
    return seq[n - 1];
}
