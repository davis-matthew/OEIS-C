/*
 * A214666  The x-coordinates of prime numbers in an Ulam spiral oriented
 *          counterclockwise with first step west.
 * https://oeis.org/A214666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 31, i16 n <= 3657, i32 n <= 9809, i64 n <= 9809,
 *            i128 n <= 9809, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214666: x‑coordinate of the nth prime encountered in a
   counterclockwise Ulam spiral starting with the first step west.
   Computation: iterate the spiral, test each number for primality
   with is_prime_small, and record the x‑coordinate when a prime is
   found. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset 1, but harmless */

    int64_t x = 0, y = 0;          /* current coordinate   */
    uint64_t num = 1;              /* value placed at (x,y)*/
    idx_t count = 0;               /* primes seen so far   */
    idx_t step = 1;                /* current step length  */

    /* Initial position (1,0) not prime, so start looping from 2 */
    while (1) {
        /* West */
        for (idx_t i = 0; i < step; ++i) {
            x -= 1;
            ++num;
            if (is_prime_small((seq_t)num)) {
                ++count;
                if (count == n) return (seq_t)x;
            }
        }
        /* North */
        for (idx_t i = 0; i < step; ++i) {
            y += 1;
            ++num;
            if (is_prime_small((seq_t)num)) {
                ++count;
                if (count == n) return (seq_t)x;
            }
        }
        ++step;   /* after two directions */

        /* East */
        for (idx_t i = 0; i < step; ++i) {
            x += 1;
            ++num;
            if (is_prime_small((seq_t)num)) {
                ++count;
                if (count == n) return (seq_t)x;
            }
        }
        /* South */
        for (idx_t i = 0; i < step; ++i) {
            y -= 1;
            ++num;
            if (is_prime_small((seq_t)num)) {
                ++count;
                if (count == n) return (seq_t)x;
            }
        }
        ++step;   /* after two directions */
    }
}
