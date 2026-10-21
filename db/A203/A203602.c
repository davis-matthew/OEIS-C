/*
 * A203602  Inverse permutation to A092401.
 * https://oeis.org/A203602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed from the OEIS A203602 formula involving
 *            alternating sums of floor(n/3^k) and the divisibility of n by
 *            powers of 3. The method follows the expression given in the OEIS
 *            entry, evaluated efficiently in O(log_3 n) time
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#if defined(__SIZEOF_INT128__)
typedef __int128 int128_t;
#else
typedef long long int128_t;
#endif

seq_t a(idx_t n)
{
    /* The OEIS definition uses a(1) as the first term; for safety handle n==0. */
    if (n == 0) return 0;

    /* Compute L = floor(log_3(n)) and the array of powers of 3. */
    idx_t pow3[65];
    pow3[0] = 1;
    size_t L = 0;
    while (pow3[L] <= n / 3) {
        ++L;
        pow3[L] = pow3[L - 1] * (idx_t)3;
    }

    /* floor_k[k] = floor(n / 3^k) for 0 <= k <= L */
    idx_t floor_k[65];
    floor_k[0] = n;
    for (size_t i = 1; i <= L; ++i) {
        floor_k[i] = floor_k[i - 1] / (idx_t)3;
    }

    /* Accumulate the sums required by the formula. */
    int128_t S2 = 0;          /* Σ_{k=0..L} (-1)^k * floor(n/3^k)        */
    int128_t F1 = 0;          /* Σ_{k=0..L-1} (-1)^k * floor(n/3^{k+1})   */
    int128_t G  = 0;          /* Σ_{k=1..L} (-1)^k * (0 if 3^k|n else -1) */
    int sign = 1;             /* (-1)^k */
    for (size_t k = 0; k <= L; ++k) {
        int128_t val = (int128_t)floor_k[k];
        S2 += sign * val;
        if (k < L) {
            int128_t valF = (int128_t)floor_k[k + 1];
            F1 += sign * valF;
        }
        if (k >= 1) {
            int128_t diff = (n % pow3[k] == 0) ? 0 : -1;
            G += sign * diff;
        }
        sign = -sign;
    }

    /* Parity term: (floor(log_3(3n)) mod 2) = (L+1) mod 2 */
    int M = (int)((L + 1) & 1);

    /* Evaluate the two parts of the analytic expression. */
    int128_t term1 = -((int128_t)2 * F1) * ((int128_t)-1 + M + G);
    int128_t term2 = ((int128_t)-1 + (int128_t)2 * S2) * ((int128_t)M + G);
    int128_t result = term1 + term2;

    return (seq_t)result;
}
