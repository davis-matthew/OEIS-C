/*
 * A043333  Numbers having one 0 in base 4.
 * https://oeis.org/A043333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers having exactly one 0 in base‑4 representation
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  compute 3^e in seq_t  */
static inline seq_t pow3_u64(idx_t e) {
    seq_t r = 1;
    while (e--) r *= 3;
    return r;
}

seq_t a(idx_t n) {
    /*  find the base‑4 length L that contains the nth number  */
    idx_t L = 1;
    seq_t cum = 0;
    while (1) {
        ++L;
        seq_t countL = (seq_t)(L - 1) * pow3_u64(L - 1);
        if (cum + countL >= n) break;
        cum += countL;
    }

    seq_t idxWithin = (seq_t)n - cum;   /* 1‑based index within block of length L  */
    seq_t value = 0;
    int zero_needed = 1;                 /* 1 if the zero has not yet been placed  */

    for (idx_t pos = 1; pos <= L; ++pos) {
        /*  candidate zero (if allowed)  */
        if (pos > 1 && zero_needed) {
            seq_t cnt = pow3_u64(L - pos);   /* zero placed now, remaining digits all non‑zero */
            if (idxWithin <= cnt) {
                value = value * 4 + 0;
                zero_needed = 0;
                continue;                    /* move to next position */
            }
            idxWithin -= cnt;
        }

        /*  candidates 1, 2, 3  */
        for (int d = 1; d <= 3; ++d) {
            seq_t cnt;
            if (zero_needed) { /* zero still to be placed somewhere in the suffix */
                idx_t remaining = L - pos;           /* positions after current one */
                if (remaining == 0) cnt = 0;         /* impossible, but keeps logic safe */
                else cnt = (seq_t)remaining * pow3_u64(remaining - 1);
            } else { /* zero already placed, all remaining digits non‑zero */
                cnt = pow3_u64(L - pos);
            }

            if (idxWithin <= cnt) {
                value = value * 4 + (seq_t)d;
                /* zero_needed unchanged */
                break;
            }
            idxWithin -= cnt;
        }
    }

    return value;
}
