/*
 * A000339  Number of partitions into non-integral powers.
 * https://oeis.org/A000339
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n^2/4)} (n^2 + 1 -
 *            ceiling(sqrt(4*n^2*k))), since for fixed x_1 = k the largest
 *            admissible x_2 is n^2 + k - ceiling(2*n*sqrt(k))
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 476, u64 n <= 7032,
 *            u128 n <= 7032, big any n
 * Helpers:   a339_ceil_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/a339_ceil_sqrt.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n * (uint64_t)n;
    uint64_t kmax = nn / 4;
    uint64_t four_nn = 4 * nn;
    seq_t total = 0;
    uint64_t part = 0;
    uint64_t k;
    for (k = 1; k <= kmax; k++) {
        uint64_t c = a339_ceil_sqrt(four_nn * k);
        part += nn + 1 - c;
        if (part >= ((uint64_t)1 << 62)) {
            total += (seq_t)part;
            part = 0;
        }
    }
    total += (seq_t)part;
    return total;
}
