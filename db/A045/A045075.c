/*
 * A045075  Numbers whose base-4 representation contains exactly two 0's and
 *          four 3's.
 * https://oeis.org/A045075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A045075: numbers whose base‑4 representation
   contains exactly two 0's and four 3's. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n=0, guard */

    idx_t count = 0;                /* how many terms we have found so far */
    seq_t low = 1;                  /* 4^(L-1) for the current length L */

    /* initialise low to 4^(6-1) = 4^5, the first length that can hold
       two 0's and four 3's. */
    for (idx_t i = 0; i < 5; ++i) low *= 4;

    for (idx_t L = 6; ; ++L) {
        seq_t high = low * 4;       /* first number with length L+1 */

        for (seq_t val = low; val < high; ++val) {
            seq_t x = val;
            uint64_t zeros = 0, threes = 0;

            /* count 0's and 3's in the base‑4 representation */
            for (idx_t i = 0; i < L; ++i) {
                seq_t digit = x % 4;
                if (digit == 0) zeros++;
                else if (digit == 3) threes++;
                x /= 4;
            }

            if (zeros == 2 && threes == 4) {
                count++;
                if (count == n) return val;   /* found the nth term */
            }
        }

        low = high;                 /* move to next length */
    }
}
