/*
 * A169967  Numbers whose decimal expansion contains only 0's and 4's.
 * https://oeis.org/A169967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=0. For n≥2 let k=n−2, L=floor(log2(k+1))+1,
 *            r=k−(2^(L−1)−1). * Then a(n)=4⋅10^(L−1)+∑_{i=0}^{L−2}[ (r>>i)&1
 *            ]⋅4⋅10^i. * This yields the sequence of all decimal numbers
 *            whose digits are only 0 or 4
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 768, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t k = n - 2;              /* index within the non‑zero part */
    /* compute L = floor(log2(k+1)) + 1 */
    idx_t L = 0;
    idx_t temp = k + 1;
    while (temp > 1) {
        temp >>= 1;
        L++;
    }
    L++;                           /* now L is the desired length */

    /* offset within numbers of length L */
    idx_t r = k - (((idx_t)1 << (L - 1)) - 1);

    /* leading digit 4 at position L−1 */
    seq_t val = (seq_t)4 * pow10(L - 1);

    /* remaining digits determined by binary bits of r */
    for (idx_t i = 0; i < (L - 1); i++) {
        if ((r >> i) & (idx_t)1)
            val += (seq_t)4 * pow10(i);
    }

    return val;
}
