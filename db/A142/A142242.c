/*
 * A142242  Row sums of A143200.
 * https://oeis.org/A142242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4 - 2^A000120(n) for n>0, a(0)=1
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    unsigned long long val = (unsigned long long)n;
    size_t ones = __builtin_popcountll(val);
    seq_t pow2 = (seq_t)1;
    for (size_t i = 0; i < ones; ++i) {
        pow2 *= 2;
    }
    return (seq_t)4 - pow2;
}
