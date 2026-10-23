/*
 * A121039  Multiples of 19 containing a 19 in their decimal representation.
 * https://oeis.org/A121039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121039: Multiples of 19 containing the decimal substring "19".
   Implementation: iterate through successive multiples of 19 and test each
   for the presence of the pattern 1 9 in the decimal representation.
   (The test is performed by scanning the digits from least to most
   significant and looking for a 9 followed by a 1 in that order.) */

#include <stdbool.h>

/* Return true iff x contains the decimal substring "19". */
static inline bool contains_19(seq_t x)
{
    if (x < (seq_t)19)            /* numbers smaller than 19 cannot contain "19" */
        return false;

    while (x >= (seq_t)10) {
        seq_t d       = x % (seq_t)10;
        seq_t next_d  = (x / (seq_t)10) % (seq_t)10;
        /* In LSB‑to‑MSB scan a forward "19" appears as a 9 followed by a 1. */
        if (d == (seq_t)9 && next_d == (seq_t)1)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset 0; harmless */

    idx_t found = 0;
    seq_t val   = (seq_t)19;             /* first positive multiple of 19 */

    while (1) {
        if (contains_19(val)) {
            ++found;
            if (found == n)
                return val;
        }
        val += (seq_t)19;                /* next multiple of 19 */
    }
}
