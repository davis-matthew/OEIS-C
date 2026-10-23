/*
 * A042640  Numerators of continued fraction convergents to sqrt(850).
 * https://oeis.org/A042640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 4898*a(n-4) - a(n-8); Initial values: a(0)=29, a(1)=175,
 *            a(2)=379, a(3)=2449, a(4)=142421, a(5)=856975, a(6)=1856371,
 *            a(7)=11995201
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Small array holding the last 8 values seen.
       a[0] .. a[7] correspond to a(i-7) .. a(i) where i is the
       current index of the last element a[7].  Initially i = 7. */
    static const seq_t init[8] = {
        (seq_t)29,
        (seq_t)175,
        (seq_t)379,
        (seq_t)2449,
        (seq_t)142421,
        (seq_t)856975,
        (seq_t)1856371,
        (seq_t)11995201
    };
    if (n < 8) {
        return init[(size_t)n];
    }

    /* Create a mutable copy of the initial values (static array) */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) {
        buf[i] = init[i];
    }

    /* Iterate from index 8 up to n inclusive */
    for (idx_t i = 8; i <= n; ++i) {
        /* Compute a(i) using the recurrence:
           a(n) = 4898*a(n-4) - a(n-8).  In the buffer,
           a(n-4) is at position 4, a(n-8) at position 0. */
        seq_t next = buf[4] * (seq_t)4898 - buf[0];

        /* Rotate the buffer left by one, discarding the oldest term
           and inserting the newly computed term at the end. */
        for (size_t j = 0; j < 7; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[7] = next;
    }

    return buf[7];  /* a(n) */
}
