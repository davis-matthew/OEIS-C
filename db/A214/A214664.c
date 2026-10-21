/*
 * A214664  The x-coordinates of prime numbers in an Ulam spiral oriented
 *          counterclockwise with first step east.
 * https://oeis.org/A214664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6495, i16 n <= 9829, i32 n <= 9829, i64 n <= 9829,
 *            i128 n <= 9829, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214664: x–coordinates of prime numbers in a counter‑clockwise Ulam spiral with first step east. */

#include <stdbool.h>

static inline bool is_prime(idx_t n) {
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n == (idx_t)0) return 0;

    seq_t x = 0, y = 0;   /* current coordinates */
    idx_t num = 1;        /* current number in the spiral */
    idx_t primes_found = 0;

    int dir = 0;          /* 0:east, 1:north, 2:west, 3:south */
    idx_t step_len = 1;   /* current step length for two directions */

    while (primes_found < n) {
        for (int side = 0; side < 2 && primes_found < n; ++side) {
            for (idx_t step = 0; step < step_len && primes_found < n; ++step) {
                switch (dir) {
                    case 0: ++x; break;   /* east */
                    case 1: ++y; break;   /* north */
                    case 2: --x; break;   /* west */
                    case 3: --y; break;   /* south */
                }
                ++num;
                if (is_prime(num)) {
                    ++primes_found;
                    if (primes_found == n) return x;
                }
            }
            dir = (dir + 1) & 3; /* next direction */
        }
        ++step_len;
    }
    return x; /* unreachable, but satisfies compiler */
}
