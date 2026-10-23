/*
 * A043979  Numbers k such that 2 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_27
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A043979:
   Numbers k such that the digits 2 and 7 occur juxtaposed in the
   base‑9 representation of k but not of k+1.
   This is implemented by a simple brute‑force scan. */

#include <stdbool.h>
#include "../utils/has_adjacent_27.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for offset 0; return 0 for safety */

    seq_t k = 1;
    idx_t count = 0;
    while (1) {
        if (has_adjacent_27(k) && !has_adjacent_27(k + 1)) {
            count++;
            if (count == n)
                return k;
        }
        k = k + 1;
    }
}
