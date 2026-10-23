/*
 * A102492  Numbers in base-20 representation that cannot be written with
 *          decimal digits.
 * https://oeis.org/A102492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sequence A102492: Numbers whose base‑20 representation contains at least one digit ≥10.
   a(n) is the n‑th positive integer with this property. */

/* Returns true if m, written in base 20, has a digit in {10,…,19}. */
static bool has_nondecimal_digit(seq_t m) {
    while (m > 0) {
        if ((m % (seq_t)20) >= (seq_t)10) {
            return true;
        }
        m /= (seq_t)20;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 1;

    while (1) {
        if (has_nondecimal_digit(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++candidate;
    }
}
