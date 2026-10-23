/*
 * A178289  Number of collinear point triples in an n X n .. X n 8-dimensional
 *          cubical grid.
 * https://oeis.org/A178289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{d=1}^{n-1} φ(d) * ((S_d)^8 - n^8) - C(n^8, 2)
 *            where S_d = n * (1 + 2 * ⌊(n-1)/d⌋) - d * ⌊(n-1)/d⌋ * (⌊(n-1)/d⌋
 *            + 1) φ(d) is Euler's totient function
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Special case n <= 1 : result is 0 */
    if (n <= 1)
        return (seq_t)0;

    /* Convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute N = n^8 */
    seq_t N = (seq_t)1;
    for (int i = 0; i < 8; ++i)
        N *= n_seq;

    /* ---------- precompute Euler's totient φ(d) for 1 <= d < n ---------- */
    idx_t *phi = (idx_t *)malloc((size_t)(n) * sizeof(idx_t));
    if (!phi)
        return (seq_t)0;  /* should never happen in the judge environment */

    for (idx_t i = 1; i < n; ++i)
        phi[i] = i;

    for (idx_t i = 2; i < n; ++i)
    {
        if (phi[i] == i) /* i is prime */
        {
            for (idx_t j = i; j < n; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* ---------- sum over d ---------- */
    seq_t sum = (seq_t)0;

    for (idx_t d = 1; d < n; ++d)
    {
        idx_t M = (n - 1) / d;                /* floor((n-1)/d) */
        seq_t M_seq = (seq_t)M;

        /* S_d = n * (1 + 2*M) - d * M * (M+1) */
        seq_t S_d = n_seq * ((seq_t)1 + (seq_t)2 * M_seq)
                    - (seq_t)d * M_seq * (M_seq + (seq_t)1);

        /* Compute S_d^8 */
        seq_t powS = (seq_t)1;
        for (int i = 0; i < 8; ++i)
            powS *= S_d;

        seq_t diff = powS - N;                /* (S_d)^8 - N */

        seq_t add = diff * (seq_t)phi[d];
        sum += add;
    }

    free(phi);

    /* divide by two (integer division, sum is always even) */
    seq_t half = sum >> 1;

    /* subtract C(N,2) = N*(N-1)/2 */
    seq_t sub = N * (N - (seq_t)1);
    sub >>= 1;

    seq_t result = half - sub;
    return result;
}
