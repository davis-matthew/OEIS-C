/*
 * A043684  a(n) = (1/2)*(n-th number whose base-2 representation has exactly
 *          8 runs).
 * https://oeis.org/A043684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the n‑th number whose binary representation has exactly
   8 runs, then return half of that number (which is always integral). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Count the number of runs (maximal blocks of equal bits) in the binary
   representation of x. */
static inline idx_t count_runs(seq_t x)
{
    if (x == 0) return 0;
    idx_t bitlen = (idx_t)bitlen_seq_t(x);      /* most significant bit + 1 */
    int prev   = -1;
    int runcnt = 0;
    /* iterate from the most significant bit down to the least significant */
    for (idx_t pos = bitlen; pos != 0; --pos) {
        int bit = (int)((x >> (pos - 1)) & 1);
        if (prev == -1) {          /* first bit */
            prev   = bit;
            runcnt = 1;
        } else if (bit != prev) {  /* run boundary */
            ++runcnt;
            prev = bit;
        }
    }
    return runcnt;
}

/* OEIS A043684: a(n) = (1/2) * (n‑th number with exactly 8 runs in base‑2). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 0;
    while (true) {
        ++x;
        if (count_runs(x) == 8) {
            ++found;
            if (found == n) {
                return x >> 1;   /* division by 2, safe because x is even */
            }
        }
    }
}
