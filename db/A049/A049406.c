/*
 * A049406  From a solution to the Prouhet-Tarry-Escott problem: 3^n + 5^n +
 *          30^n + 57^n + 104^n + 116^n + 186^n + 198^n + 245^n + 272^n +
 *          297^n + 299^n.
 * https://oeis.org/A049406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n + 5^n + 30^n + 57^n + 104^n + 116^n + 186^n + 198^n +
 *            245^n + 272^n + 297^n + 299^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    static const uint64_t bases[12] = {
        3, 5, 30, 57, 104, 116, 186, 198, 245, 272, 297, 299
    };
    seq_t sum = 0;
    for (int i = 0; i < 12; ++i)
        sum += seq_pow((seq_t)bases[i], n);
    return sum;
}
