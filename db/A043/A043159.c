/*
 * A043159  Numbers k such that 1 and 2 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search. */
#include <stdbool.h>

static bool has_adj12(seq_t k)
{
    bool seen = false;
    seq_t prev = 0;
    while (k != (seq_t)0) {
        seq_t d = k % (seq_t)8;
        if (seen) {
            if ((d == (seq_t)1 && prev == (seq_t)2) ||
                (d == (seq_t)2 && prev == (seq_t)1))
                return true;
        }
        prev = d;
        seen = true;
        k /= (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = (seq_t)1;
    while (1) {
        if (has_adj12(k) && !has_adj12(k - (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
