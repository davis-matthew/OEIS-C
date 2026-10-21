/*
 * A059011  Odd number of 0's and 1's in binary expansion.
 * https://oeis.org/A059011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen64, popcount64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059011: numbers whose binary expansion has an odd
 * number of 0's and an odd number of 1's.  Computed by a simple
 * brute‑force search – iterate through integers, test the parity
 * of ones and zeros, and count until the requested index is reached. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen64.h"
#include "../utils/popcount64.h"

extern seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 as sentinel */
    uint64_t found = 0;
    for (uint64_t x = 2; ; ++x) {   /* start from 2, the first valid term */
        unsigned ones  = popcount64(x);
        unsigned bits  = bitlen64(x);
        unsigned zeros = bits - ones;

        if ( (ones & 1) && (zeros & 1) ) {
            ++found;
            if (found == n) {
                return (seq_t)x;
            }
        }
    }
}
