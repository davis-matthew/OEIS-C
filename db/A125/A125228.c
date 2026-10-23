/*
 * A125228  Maximal number of squares of side 1 in a disk of radius n.
 * https://oeis.org/A125228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = 2*∑_{k=1}^{n-1} floor(2*sqrt(n²-(k+1/2)²)) + 2*n - 1 *
 *            * Using integer arithmetic: * floor(2*sqrt(x)) = floor(sqrt(4x))
 *            * 4x = 4n² - (2k+1)² * Thus a(n) = 2*∑_{k=1}^{n-1}
 *            floor_sqrt(4*n² - (2k+1)²) + 2*n - 1
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; we return 0 for n == 0 as a safe value */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t four_n_sq = (seq_t)4 * nn * nn;   /* 4 * n² */
    seq_t sum = 0;

    for (idx_t k = 1; k < n; ++k) {
        seq_t two_k_plus1 = (seq_t)(2 * k + 1);        /* 2k+1 */
        seq_t inner = four_n_sq - two_k_plus1 * two_k_plus1; /* 4n² - (2k+1)² */
        seq_t s = floor_sqrt(inner);                  /* floor(sqrt(inner)) */
        sum += s;
    }

    seq_t result = (seq_t)2 * sum + (seq_t)2 * nn - (seq_t)1;
    return result;
}
