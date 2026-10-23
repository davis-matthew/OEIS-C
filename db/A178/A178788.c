/*
 * A178788  Characteristic function of numbers having distinct digits in their
 *          decimal representation.
 * https://oeis.org/A178788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if n has all distinct decimal digits, 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* static helper that returns true if all decimal digits of n are distinct. */
static bool has_distinct_digits(idx_t n) {
    unsigned mask = 0;
    if (n == 0)   /* single digit 0 is distinct */
        return true;

    while (n > 0) {
        unsigned d = n % 10u;      /* extract last digit */
        if (mask & (1u << d))
            return false;          /* duplicate digit found */
        mask |= 1u << d;           /* record digit */
        n /= 10u;
    }
    return true;
}

seq_t a(idx_t n) {
    return has_distinct_digits(n) ? (seq_t)1 : (seq_t)0;
}
