/*
 * A175325  a(n) = the smallest positive multiple of n such that a(n) written
 *          in binary contains only distinct run-lengths (of runs both of 0's
 *          and of 1's).
 * https://oeis.org/A175325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175325: smallest positive multiple of n whose binary
   representation has all run‑lengths distinct.  The routine
   searches successive multiples of n until the condition holds. */

#include <stdbool.h>
#include <string.h>

/* Check whether the binary representation of x contains only
   distinct run‑lengths.  Works for any seq_t that supports
   ordinary integer operators and bit shifts. */
static bool has_distinct_runs(seq_t x)
{
    /* 128 is enough for seq_t up to 128 bits; it also covers
       the 64‑bit case comfortably. */
    static char seen[128];
    int last_bit = -1;
    int runlen = 0;

    /* reset the seen array */
    memset(seen, 0, sizeof(seen));

    /* iterate over the bits of x, from LSB to MSB */
    while (x != (seq_t)0) {
        int bit = (int)(x & 1);
        if (bit == last_bit) {
            runlen++;
        } else {
            if (last_bit != -1) {
                /* if the same run length appeared before,
                   the property fails */
                if (seen[runlen]) return false;
                seen[runlen] = 1;
            }
            last_bit = bit;
            runlen = 1;
        }
        x >>= 1;
    }

    /* process the final run */
    if (last_bit != -1) {
        if (seen[runlen]) return false;
        seen[runlen] = 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined only for n ≥ 1 */

    idx_t k = 1;
    for (;;) {
        /* compute n*k using 64‑bit arithmetic to avoid overflow
           when seq_t is larger than 64 bits */
        uint64_t prod = (uint64_t)n * (uint64_t)k;
        seq_t cand = (seq_t)prod;
        if (has_distinct_runs(cand)) return cand;
        k++;
    }
}
