/*
 * A213345  3-quantum transitions in systems of N>=3 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213345
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) = 2^(N-3-2*k) * binomial(N,k) * binomial(N-k,3+k), where N
 *            is the smallest integer ≥3 such that n ≤ Σ_{m=3}^{N}
 *            (floor((m-1)/2)), and k = n - 1 - Σ_{m=3}^{N-1}
 *            (floor((m-1)/2)). The sequence is laid out row‑major: for each
 *            N=3,4,5,… list all k = 0…floor((N-3)/2). This function
 *            calculates a(n...
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 76, u64 n <= 281,
 *            u128 n <= 1104, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;              /* undefined for n<3, return 0 */

    /* Find the row N and column k for the given index n. */
    idx_t rem = n - 3;                /* remaining offset within the triangle */
    idx_t N = 3;
    while (1) {
        idx_t cols = (N - 1) / 2;     /* number of terms in row N */
        if (rem < cols) break;        /* found the correct row */
        rem -= cols;
        ++N;
    }
    idx_t k = rem;                    /* column index within the row */

    /* Compute the value: T(3;N,k) = 2^(N-3-2*k) * C(N,k) * C(N-k,3+k) */
    idx_t exp = N - 3 - 2 * k;        /* exponent of 2, guaranteed ≥0 */

    seq_t val = pow2(exp);            /* 2^exp */
    val = val * binomial(N, k);       /* * C(N,k) */
    val = val * binomial(N - k, 3 + k); /* * C(N-k,3+k) */

    return val;
}
