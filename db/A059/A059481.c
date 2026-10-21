/*
 * A059481  Triangle read by rows. T(n, k) = binomial(n+k-1, k) for 0 <= k <=
 *          n.
 * https://oeis.org/A059481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 64, u32 n <= 188, u64 n <= 663,
 *            u128 n <= 2343, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059481: Read the triangle T(n,k)=C(n+k-1,k) row‑by‑row.
   For a given index n, determine the row r and column k such that
   n = r*(r+1)/2 + k (0 ≤ k ≤ r) and return C(r+k-1, k). */
#include <stdint.h>
#include "../utils/gcd.h"     /* gcd(seq_t, seq_t) -- needed for safe binomial */

/* Safe multiplicative binomial for all idx_t arguments and a seq_t result */
static seq_t binom_seq(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;                /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* Build numerator and denominator for this step */
        seq_t num = (seq_t)(N - K + i);
        seq_t den = (seq_t)i;

        /* Cancel gcd between numerator and denominator */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Cancel gcd between current result and remaining denominator */
        seq_t g2 = gcd(result, den);
        result /= g2;
        den    /= g2;

        /* Multiply by the reduced numerator and divide by any leftover denom */
        result *= num;
        if (den != 1)
            result /= den;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low  = 0;
    idx_t high = n + 1;            /* r never exceeds n+1 */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    idx_t r = high;                       /* row index */
    idx_t prev_total = r * (r + 1) / 2;    /* total elements before row r */
    idx_t k = n - prev_total;             /* column within the row */

    return binom_seq(r + k - 1, k);        /* T(r,k) = C(r+k-1,k) */
}
