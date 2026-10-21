/*
 * A211157  Number of 2 X 2 matrices having all terms in {-n,...,0,..,n} and
 *          positive even determinant.
 * https://oeis.org/A211157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((N^2 - O^2)^2 + O^4 - ((2N-1)^2 + 8 * Σ_{t=1}^{n}
 *            C(t)*⌊n/t⌋^2)) / 2 where N = 2n+1, E = 2⌊n/2⌋+1, O = N - E,
 *            C(1)=1, C(t)=2·φ(t) for t>1, and φ is Euler's totient
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Basic parameters */
    seq_t N = (seq_t)2 * (seq_t)n + 1;
    seq_t E = (seq_t)2 * (seq_t)(n / 2) + 1;
    seq_t O = N - E;

    /* Total number of even‑determinant matrices */
    seq_t totalEven = (N * N - O * O) * (N * N - O * O) + O * O * O * O;

    /* Zero determinant count: base term with a=d=0 or b=c=0 */
    seq_t zeroBase = (seq_t)(2 * N - 1) * (seq_t)(2 * N - 1);

    /* Allocate Totient array (size n+1) */
    uint64_t *phi = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    if (!phi)
        return 0;                    /* fallback on allocation failure */

    /* Euler totient sieve up to n */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = i;
    for (idx_t i = 2; i <= n; ++i)
        if (phi[i] == i)
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;

    /* Sum Σ C(t)*⌊n/t⌋² */
    seq_t sumC = 0;
    for (idx_t t = 1; t <= n; ++t) {
        idx_t q = n / t;                     /* floor(n/t) */
        seq_t floorSq = (seq_t)(q * q);       /* ⌊n/t⌋² as seq_t */
        seq_t c = (t == 1) ? 1 : (seq_t)((uint64_t)2 * phi[t]); /* C(t) */
        sumC += c * floorSq;
    }

    seq_t zeroCount = zeroBase + (seq_t)8 * sumC;
    free(phi);

    /* Desired count: positive even determinants only */
    seq_t result = (totalEven - zeroCount) / 2;
    return result;
}
