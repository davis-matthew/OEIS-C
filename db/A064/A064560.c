/*
 * A064560  Numbers n such that reciprocal of n terminates with an infinite
 *          repetition of digit 1. Multiples of 10 are omitted.
 * https://oeis.org/A064560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sequence A064560 – numbers n such that 1/n has a decimal expansion
 * ending in an infinite repetition of the digit 1.
 * These are exactly 9·2^(6i) and 9·5^(6i) (i ≥ 0), merged in
 * increasing order with the common initial term 9 kept only once.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    seq_t a_val = 9;           /* current value of 9·2^(6i) */
    seq_t b_val = 9;           /* current value of 9·5^(6i) */
    bool a_exhausted = false; /* true if a_val would overflow seq_t */
    bool b_exhausted = false; /* true if b_val would overflow seq_t */
    idx_t count   = 0;         /* number of terms generated so far */

    for (;;) {
        seq_t next;

        /* Choose the smallest available candidate */
        if (!a_exhausted && !b_exhausted) {
            next = (a_val <= b_val) ? a_val : b_val;
        } else if (!a_exhausted) {
            next = a_val;
        } else if (!b_exhausted) {
            next = b_val;
        } else {
            /* No more values that fit in seq_t */
            return 0;
        }

        if (++count == n) return next;

        /* Advance the progression(s) that produced this value */
        if (!a_exhausted && a_val == next) {
            seq_t prod = a_val * (seq_t)64;          /* × 2^6 */
            if (a_val && prod / 64 != a_val) {
                a_exhausted = true;                  /* would overflow */
            } else {
                a_val = prod;
            }
        }
        if (!b_exhausted && b_val == next) {
            seq_t prod = b_val * (seq_t)15625;       /* × 5^6 */
            if (b_val && prod / 15625 != b_val) {
                b_exhausted = true;                  /* would overflow */
            } else {
                b_val = prod;
            }
        }
    }
}
