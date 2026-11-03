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
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A043979:
   Numbers k such that the digits 2 and 7 occur juxtaposed in the
   base‑9 representation of k but not of k+1.
   This is implemented by a simple brute‑force scan. */

#include <stdbool.h>

static inline bool has_adjacent_27(seq_t x)
{
    bool first = true;
    seq_t prev = 0;          /* dummy initialization */
    while (x != 0) {
        seq_t d = x % 9;
        x = x / 9;
        if (first) {
            prev = d;
            first = false;
        } else {
            if ((prev == 2 && d == 7) || (prev == 7 && d == 2))
                return true;
            prev = d;
        }
    }
    return false;
}

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
