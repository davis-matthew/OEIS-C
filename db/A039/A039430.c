/*
 * A039430  Numbers whose base-10 representation has the same nonzero number
 *          of 5's and 8's.
 * https://oeis.org/A039430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force enumeration of integers whose base‑10 representation
 * contains an equal non‑zero number of digits 5 and 8, as defined for OEIS
 * sequence A039430.                                    */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

static bool equal_fives_and_eights(seq_t v)
{
    unsigned freq[10];
    digit_frequencies(v, freq);
    return freq[5] > 0 && freq[5] == freq[8];
}

seq_t a(idx_t n)
{
    if (n == 0)    /* offset 1 – undefined value below 1 */
        return 0;

    seq_t val = 1;
    idx_t  hit = 0;

    while (1) {
        if (equal_fives_and_eights(val)) {
            ++hit;
            if (hit == n)
                return val;
        }
        ++val;        /* advance to next integer */
    }
}
