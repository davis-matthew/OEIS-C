/*
 * A096271  Ternary sequence that is a fixed point of the morphism 0 -> 01, 1
 *          -> 02, 2 -> 00.
 * https://oeis.org/A096271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(2n) = 0; a(2n+1) = (a(n) + 1) mod 3 This recurrence
 *            directly implements the definition of sequence A096271
 * Range:     u8 n <= 8986, u16 n <= 8986, u32 n <= 8986, u64 n <= 8986,
 *            u128 n <= 8986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Count how many times we encounter an odd index while repeatedly
       applying n -> (n-1)/2. The final value is that count modulo 3.   */
    unsigned count = 0;                /* small enough for all counts (≤64) */
    while (n > 0) {
        if ((n & 1ULL) == 0)          /* even index: result is 0 */
            break;
        ++count;
        n = (n - 1) >> 1;             /* n = (n-1)/2 for odd n */
    }
    return (seq_t)(count % 3);
}
