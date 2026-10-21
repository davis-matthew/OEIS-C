/*
 * A057347  Leap years in the Islamic calendar starting year 1 AH (Anno
 *          Hegirae) = 622 CE (Common Era or AD). There are 11 leap years in a
 *          30 year cycle.
 * https://oeis.org/A057347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((30*n - 4)/11)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute 30*n - 4 in a wider type to avoid overflow for narrow seq_t types */
    unsigned long long prod = 30ULL * (unsigned long long)n;
    unsigned long long temp = prod - 4ULL;
    unsigned long long res64 = temp / 11ULL;
    return (seq_t)res64;
}
