/*
 * A194881  A number of sum-free sets related to fractional parts of multiples
 *          of a rational number in the range 1/3 to 2/3.
 * https://oeis.org/A194881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + \sum_{j=1}^{n} \phi(3j)/2
 * Range:     u8 n <= 26, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   sieve_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where \phi is Euler's totient function, A000010) */

#include <stdlib.h>
#include "../utils/sieve_phi.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, behaviour below not required */
        return 0;

    idx_t limit = 3 * n;   /* need phi values up to 3*n */
    uint64_t *phi = sieve_phi(limit);
    if (!phi)
        return 0;          /* allocation failed, return a safe value */

    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        uint64_t v = phi[(size_t)(3 * j)];   /* phi(3*j) */
        sum += (seq_t)(v / 2);               /* integer division by 2 */
    }

    free(phi);
    return sum + (seq_t)1;
}
