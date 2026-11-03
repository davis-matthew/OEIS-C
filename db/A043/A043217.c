/*
 * A043217  Numbers k such that 0 and 2 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal representation of k contains
 * '02' or '20', but the decimal representation of k-1 does not. */

#include <stdbool.h>

static bool has_adjacent_0_2(seq_t x) {
    /* Return true if x has a pair of adjacent digits 0 and 2 in any order. */
    if (x < 10) return false;            /* single digit cannot contain adjacency */
    seq_t prev = x % 10;                  /* least–significant digit */
    x /= 10;
    while (x > 0) {
        seq_t cur = x % 10;
        if ((prev == 0 && cur == 2) || (prev == 2 && cur == 0))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    seq_t k = 1;        /* start scanning from 1 */
    idx_t count = 0;    /* how many valid k have been found */

    while (1) {
        if (has_adjacent_0_2(k)) {
            seq_t km1 = k - 1;                 /* previous number */
            if (!has_adjacent_0_2(km1)) {
                ++count;                       /* found a term of the sequence */
                if (count == n) return k;      /* return the nth term */
            }
        }
        ++k;                                   /* check next integer */
    }
}
