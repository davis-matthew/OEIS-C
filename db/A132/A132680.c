/*
 * A132680  Number of ones in binary representation of odious numbers.
 * https://oeis.org/A132680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of 1s in binary representation of the nth odious
 *            number
 * Range:     u8 n <= 10001, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 121 terms through n = 9831 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return a(n)  -- 1-indexed */
seq_t a(idx_t n) {
    idx_t count = 0;        /* number of odious numbers seen so far */
    idx_t num = 0;          /* current integer being tested */

    while (count < n) {
        ++num;
        int ones = __builtin_popcountll(num); /* popcount of num */
        if (ones & 1) {                      /* odious number? */
            ++count;
            if (count == n)
                return (seq_t)ones;
        }
    }
    /* unreachable, but required to satisfy the compiler */
    return (seq_t)0;
}
