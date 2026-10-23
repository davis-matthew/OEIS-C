/*
 * A039024  Numbers whose base-6 representation has the same number of 2's and
 *          3's.
 * https://oeis.org/A039024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* This function returns the n‑th number (1‑based indexing) whose
   base‑6 representation contains exactly the same number of digits
   '2' and '3'.
   The method is a straightforward brute‑force search. */

static bool balanced(seq_t v) {
    int c2 = 0, c3 = 0;
    while (v) {
        int d = (int)(v % 6);
        if (d == 2)
            ++c2;
        else if (d == 3)
            ++c3;
        v /= 6;
    }
    return c2 == c3;
}

seq_t a(idx_t n) {
    seq_t x = 0;
    idx_t count = 0;
    while (1) {
        if (balanced(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
