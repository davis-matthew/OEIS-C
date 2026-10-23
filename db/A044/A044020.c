/*
 * A044020  Numbers k such that 3 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adj34_43
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A044020: numbers k such that the decimal representation of k contains
    the substring 34 or 43, but the decimal representation of k+1 does not.
    The function returns the n‑th such number, with offset 1 (a(1)=34). */
#include <stdbool.h>
#include "../utils/has_adj34_43.h"

seq_t a(idx_t n) {
    idx_t count = 0;        /* number of qualifying k's found */
    seq_t k = 0;            /* current candidate */
    while (1) {
        if (has_adj34_43(k) && !has_adj34_43(k + (seq_t)1)) {
            count++;
            if (count == n) return k;
        }
        k++;
    }
}
