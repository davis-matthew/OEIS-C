/*
 * A002543  Complete Post functions of n variables.
 * https://oeis.org/A002543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (1/n!) * Sum_{s in S_n} ( 2^(c(s)-2) - [s has a cycle of
 *            odd length] * 2^(c(s)/2-1) ), where c(s) = number of cycles of s
 *            acting on {0,1}^n = (1/L) * Sum_{d|L} phi(L/d) * 2^(Sum_i
 *            gcd(l_i,d)), with l_i the cycle lengths of s and L = lcm(l_i).
 *            (Burnside count of Sheffer (complete) Boolean f...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   a2543_gcd, a2543_leaf, a2543_pow2, a2543_rec, a2543_tot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2543_gcd.h"
#include "../utils/a2543_leaf.h"
#include "../utils/a2543_pow2.h"
#include "../utils/a2543_rec.h"
#include "../utils/a2543_tot.h"

seq_t a(idx_t n)
{
    seq_t fact = 1, total = 0;
    idx_t i;
    int *parts;
    for (i = 2; i <= n; i++) fact *= (seq_t)i;
    parts = (int *)malloc(sizeof(int) * (size_t)(n + 1));
    a2543_rec((int)n, (int)n, parts, 0, fact, &total);
    free(parts);
    return total / fact;
}
