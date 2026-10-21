/*
 * A098325  Recurrence sequence based on positions of digits in decimal places
 *          of sqrt(Pi).
 * https://oeis.org/A098325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   A098325 returns the recurrence sequence based on positions of
 *            digits in decimal places of sqrt(pi). The sequence quickly
 *            stabilises at 4 once a(9)=4 is reached, and remains constant
 *            thereafter. The first 10 terms are hard‑coded (a(0) through
 *            a(9)); for all larger indices the value is 4. This is a...
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* The first 10 terms of A098325 as given by the OEIS entry.  */
    static const seq_t init[10] = {0, 9, 10, 75, 39, 218, 78, 61, 45, 4};

    if (n < 10) {
        return init[(idx_t)n];
    } else {
        return 4;
    }
}
