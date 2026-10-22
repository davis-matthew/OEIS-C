/*
 * A006237  Number of spanning trees of n-cube.
 * https://oeis.org/A006237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2^(2^n-1-n) * Product_{k=1..n} k^binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a006237_powseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006237_powseq.h"

seq_t a(idx_t n) {
    uint64_t e2, c;
    seq_t result;
    idx_t k;

    if (n >= 64) e2 = UINT64_MAX;
    else e2 = ((uint64_t)1 << n) - 1 - (uint64_t)n;
    result = a006237_powseq((seq_t)2, e2);

    c = 1; /* binomial(n, k), built multiplicatively */
    for (k = 1; k <= n; k++) {
        c = (uint64_t)(((unsigned __int128)c * (n - k + 1)) / k);
        if (k >= 2) result *= a006237_powseq((seq_t)k, c);
    }
    return result;
}
