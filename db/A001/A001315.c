/*
 * A001315  a(n) = Sum_{k=0..n} 2^binomial(n,k).
 * https://oeis.org/A001315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^binomial(n,k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   a001315_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001315_pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t b = 1;      /* binomial(n,k) */
    int sat = 0;         /* binomial exceeded 64 bits */
    idx_t half = n / 2;
    idx_t k;
    for (k = 0; k <= half; k++) {
        seq_t p = sat ? (seq_t)0 : a001315_pow2(b);
        if (2 * k == n)
            sum += p;
        else
            sum += p + p;
        if (!sat && k < half) {
            unsigned __int128 t = (unsigned __int128)b * (unsigned __int128)(n - k);
            t /= (unsigned __int128)(k + 1);
            if (t > (unsigned __int128)UINT64_MAX)
                sat = 1;
            else
                b = (uint64_t)t;
        }
    }
    return sum;
}
