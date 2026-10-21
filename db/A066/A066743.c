/*
 * A066743  a(n) is the number of integers of the form (n^2+1)/(k^2+1), where
 *          k = 1,2,3,...,n.
 * https://oeis.org/A066743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count_{k=1..n} [(n^2+1) mod (k^2+1) == 0]
 * Range:     u8 n <= 16, u16 n <= 779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t count = 0;
    seq_t N2 = (seq_t)n * (seq_t)n + 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t K2 = (seq_t)k * (seq_t)k + 1;
        if (N2 % K2 == 0) {
            ++count;
        }
    }
    return count;
}
