/*
 * A116451  Numbers having fewer prime factors than at least one smaller
 *          number.
 * https://oeis.org/A116451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116451: numbers whose odd part is >3 (complement of A029744)
   A(n) = all positive integers except numbers of the form 2^k or 3·2^k.
   The sequence is the increasing list of such numbers, starting with a(0)=5. */

#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;      // number of accepted terms found so far
    idx_t candidate = 1;  // current integer to test

    while (1) {
        // Reject if candidate is a power of two
        if ((candidate & (candidate - 1)) != 0) {
            // Reject if candidate is 3 times a power of two
            if (!(candidate % 3 == 0 && ((candidate / 3) & ((candidate / 3) - 1)) == 0)) {
                if (count == n)
                    return (seq_t)candidate;
                ++count;
            }
        }
        ++candidate;
    }
}
