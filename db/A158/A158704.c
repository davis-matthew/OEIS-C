/*
 * A158704  Nonnegative integers with an even number of even powers of 2 in
 *          their base-2 representation.
 * https://oeis.org/A158704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th nonnegative integer whose binary representation
 *            contains an even number of 1-bits in even positions (positions
 *            start from 0)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8993 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether the number of set bits in even positions of x is even */
static bool even_even_bits_parity(seq_t x) {
    unsigned parity = 0;
    seq_t t = x;
    uint8_t pos = 0;
    while (t != 0) {
        if ((pos & 1) == 0 && (t & 1))
            parity ^= 1;
        t >>= 1;
        pos++;
    }
    return parity == 0;
}

seq_t a(idx_t n) {
    /* n is 1-based: a(1) is the first term (0) */
    seq_t k = 0;
    idx_t found = 0;
    for (;;) {
        if (even_even_bits_parity(k)) {
            found++;
            if (found == n)
                return k;
        }
        k++;
    }
}
