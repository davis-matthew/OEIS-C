/*
 * A000827  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(6*n!)) * Sum_{sigma in S_n} Sum_{tau in S_3}
 *            Prod_{cycles of sigma on {0,1}^n, length L} 2^{cyc(tau^L)}
 *            (Burnside: 3-output Boolean maps of n inputs, S_n on inputs x
 *            S_3 on outputs). For each cycle type of sigma with C cycles on
 *            {0,1}^n, O of odd length, T of length divisible by 3: con...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   g827_gcd, leaf827, p827_pow2, rec827
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/g827_gcd.h"
#include "../utils/leaf827.h"
#include "../utils/p827_pow2.h"
#include "../utils/rec827.h"

seq_t a(idx_t n)
{
    int *parts = (int *)malloc(sizeof(int) * (size_t)(n + 1));
    seq_t S = 0;
    rec827((int)n, (int)n, parts, 0, n, &S);
    free(parts);
    S /= 6;
    idx_t t;
    for (t = 2; t <= n; t++) S /= (seq_t)t;
    return S;
}
