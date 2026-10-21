/*
 * A066626  Concatenation of cubes in increasing order up to the n-th and then
 *          in decreasing order.
 * https://oeis.org/A066626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat_{i=1}^{n} i^3 ++ concat_{i=n-1}^{1} i^3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* ascending part: 1^3, 2^3, ..., n^3 */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t cube = (uint64_t)i * (uint64_t)i * (uint64_t)i;

        /* number of decimal digits of cube */
        int digits = 0;
        uint64_t t = cube;
        do {
            ++digits;
            t /= 10;
        } while (t);

        /* multiplier = 10^digits */
        seq_t mult = 1;
        for (int d = 0; d < digits; ++d) {
            mult *= 10;
        }

        result = result * mult + (seq_t)cube;
    }

    /* descending part: (n-1)^3, (n-2)^3, ..., 1^3 */
    for (int64_t i = (int64_t)n - 1; i >= 1; --i) {
        uint64_t cube = (uint64_t)i * (uint64_t)i * (uint64_t)i;

        int digits = 0;
        uint64_t t = cube;
        do {
            ++digits;
            t /= 10;
        } while (t);

        seq_t mult = 1;
        for (int d = 0; d < digits; ++d) {
            mult *= 10;
        }

        result = result * mult + (seq_t)cube;
    }

    return result;
}
