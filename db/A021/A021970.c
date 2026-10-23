/*
 * A021970  Decimal expansion of 1/966.
 * https://oeis.org/A021970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of 1/966 (OEIS A021970) computed by long division. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state to cache the period of 1/966. */
    static bool initialized = false;
    static seq_t *digits = NULL;        /* Stores one full pre‑period + one cycle. */
    static unsigned preperiod_len = 0;   /* Length of the non‑repeating prefix. */
    static unsigned cycle_len = 0;       /* Length of the repeating cycle (0 if terminating). */
    static unsigned digits_len = 0;      /* Total number of pre‑period + cycle digits stored. */

    if (!initialized) {
        const unsigned m = 966;                 /* Denominator. */
        int rem_pos[m];                         /* remainder → position map (−1 = unseen). */
        for (unsigned i = 0; i < m; ++i) rem_pos[i] = -1;

        digits = (seq_t *)malloc(1000 * sizeof(seq_t));  /* Max possible length ≈ m. */
        unsigned pos = 0;                            /* Next index to store a digit. */
        unsigned remainder = 1 % m;

        while (1) {
            if (remainder == 0) {                  /* Terminating decimal. */
                preperiod_len = pos;
                cycle_len = 0;
                digits_len = pos;
                break;
            }
            if (rem_pos[remainder] != -1) {         /* Cycle detected. */
                preperiod_len = rem_pos[remainder];
                cycle_len = pos - rem_pos[remainder];
                digits_len = pos;                  /* Stored digits = preperiod + one cycle. */
                break;
            }
            rem_pos[remainder] = pos;               /* Remember this remainder. */
            unsigned digit = (remainder * 10) / m;  /* Next decimal digit. */
            digits[pos++] = (seq_t)digit;
            remainder = (remainder * 10) % m;
        }

        initialized = true;
    }

    /* Fast answer using the cached period. */
    if (n < digits_len) {
        return digits[(size_t)n];
    } else if (cycle_len == 0) {                 /* After termination, all zeros. */
        return 0;
    } else {
        idx_t offset = n - preperiod_len;
        unsigned idx_in_cycle = (unsigned)(offset % cycle_len);
        return digits[preperiod_len + idx_in_cycle];
    }
}
