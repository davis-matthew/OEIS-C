/*
 * A123652  a(n) = 1 + n^2 + n^3 + n^5 + n^7 + n^11 + n^13 + n^17 + n^19 +
 *          n^23 + n^29 + n^31 + n^37 + n^41.
 * https://oeis.org/A123652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^2 + n^3 + n^5 + n^7 + n^11 + n^13 + n^17 + n^19 +
 *            n^23 + n^29 + n^31 + n^37 + n^41
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = 1;                         /* the constant 1 */
    const unsigned int exps[] = {2, 3, 5, 7, 11, 13, 17,
                                 19, 23, 29, 31, 37, 41};
    const size_t cnt = sizeof(exps) / sizeof(exps[0]);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t power = 1;
        unsigned int e = exps[i];
        for (unsigned int r = 0; r < e; ++r) {
            power *= nn;
        }
        result += power;
    }

    return result;
}
