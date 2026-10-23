/*
 * A121023  Multiples of 3 containing a 3 in their decimal representation.
 * https://oeis.org/A121023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 3440, u32 n <= 3440, u64 n <= 3440,
 *            u128 n <= 3440, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: multiples of 3 that contain a digit 3 (OEIS A121023) */
/* Computed by brute‑force search for each call. */
#include <stdint.h>

static inline int has_digit_three(seq_t x)
{
    while (x > 0) {
        if (x % 10 == 3)
            return 1;
        x /= 10;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for offset 1 */

    seq_t count = 0;
    seq_t cand   = 3;            /* first multiple of 3 */

    while (count < n) {
        if (has_digit_three(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        cand += 3;                /* next multiple of 3 */
    }

    /* should never reach here */
    return 0;
}
