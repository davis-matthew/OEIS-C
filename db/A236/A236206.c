/*
 * A236206  Numbers not divisible by 3, 5 or 7.
 * https://oeis.org/A236206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236206 – Numbers not divisible by 3, 5 or 7.
   Formula: a(n) is the nth positive integer that is coprime to 105. */

/* The implementation scans the natural numbers and counts those
   not divisible by 3, 5, or 7 until it reaches the requested index n. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1, but guard against misuse */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;   /* number of qualifying integers found so far */

    for (idx_t num = 1; ; ++num) {
        if ((num % 3 != 0) && (num % 5 != 0) && (num % 7 != 0)) {
            ++count;
            if (count == n)
                return (seq_t)num;   /* cast to seq_t for return */
        }
    }
}
