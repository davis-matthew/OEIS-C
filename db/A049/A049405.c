/*
 * A049405  From a solution to the Prouhet-Tarry-Escott problem: 0^n + 11^n +
 *          24^n + 65^n + 90^n + 129^n + 173^n + 212^n + 237^n + 278^n + 291^n
 *          + 302^n.
 * https://oeis.org/A049405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0^n + 11^n + 24^n + 65^n + 90^n + 129^n + 173^n + 212^n +
 *            237^n + 278^n + 291^n + 302^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    static const seq_t bases[12] = {
        0,   11,  24,  65,  90,  129,
        173, 212, 237, 278, 291, 302
    };

    seq_t sum = 0;
    for (size_t i = 0; i < 12; ++i)
        sum += big_pow(bases[i], n);   /* big_pow handles 0^0 = 1  */

    return sum;
}
