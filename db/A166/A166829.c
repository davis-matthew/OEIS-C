/*
 * A166829  Number of nX2 1..7 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nonincreasing order.
 * https://oeis.org/A166829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(n-2)*(n-3) * (n^9 + 78*n^8 + 3141*n^7 + 75084*n^6
 *            + 994614*n^5 + 5642142*n^4 - 2217436*n^3 - 75779064*n^2 +
 *            77727420*n - 2494800) / 14968800 (a(1)=a(2)=a(3)=0)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    seq_t N = (seq_t)n;

    /* Polynomial P(n) via Horner's rule.  Coefficients are
       a9=1, a8=78, a7=3141, a6=75084, a5=994614,
       a4=5642142, a3=-2217436, a2=-75779064,
       a1=77727420, a0=-2494800        */
    static const long long coeffs[10] = {
        1LL, 78LL, 3141LL, 75084LL, 994614LL,
        5642142LL, -2217436LL, -75779064LL,
        77727420LL, -2494800LL
    };

    seq_t P = (seq_t)coeffs[0];
    for (int i = 1; i < 10; ++i)
        P = P * N + (seq_t)coeffs[i];

    /* Compute (n-1)*(n-2)*(n-3)*P */
    seq_t prod = (N - 1) * (N - 2);
    prod = prod * (N - 3);
    prod = prod * P;

    /* Divide by 14968800 */
    const seq_t DEN = (seq_t)14968800;
    return prod / DEN;
}
