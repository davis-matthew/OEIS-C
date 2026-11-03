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
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where \phi is Euler's totient function, A000010) */

#include <stdlib.h>

static uint64_t *sieve_phi(idx_t up)
{
    /* Allocate array of size up+1 (up may be 0) */
    uint64_t *phi = (uint64_t *)malloc((size_t)(up + 1) * sizeof(uint64_t));
    if (!phi) return NULL;

    for (idx_t i = 0; i <= up; ++i)
        phi[i] = i;                     /* phi of a prime will stay unchanged */

    for (idx_t i = 2; i <= up; ++i) {
        if (phi[i] == i) {              /* i is prime */
            for (idx_t j = i; j <= up; j += i) {
                phi[j] = (phi[j] / i) * (i - 1);
            }
        }
    }
    return phi;
}

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
