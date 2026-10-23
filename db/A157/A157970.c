/*
 * A157970  Evil twin locations: first members of pairs of consecutive evil
 *          numbers.
 * https://oeis.org/A157970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A248056(n) - 1 (first member of each pair of consecutive
 *            evil numbers)
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_evil(seq_t x) {
    /* Count set bits and return true if number of 1's is even. */
    unsigned int count = 0;
    while (x) {
        count += (unsigned int)(x & 1);
        x >>= 1;
    }
    return (count & 1) == 0;
}

seq_t a(idx_t n) {
    seq_t i = 0;          /* current integer being examined */
    bool prev_is_evil = false; /* whether the previous integer was evil */
    seq_t prev = 0;           /* previous integer value */
    idx_t pairs = 0;          /* number of evil twin pairs found */

    while (pairs < n) {
        bool curr_is_evil = is_evil(i);

        if (prev_is_evil && curr_is_evil) {
            pairs++;
            if (pairs == n)
                return prev;
        }

        prev_is_evil = curr_is_evil;
        prev = i;
        i++;
    }

    /* Should never reach here because a(n) exists for all n >= 1. */
    return 0;
}
