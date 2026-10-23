/*
 * A187791  Repeat n+1 times 2^A005187(n).
 * https://oeis.org/A187791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, less
 * Formula:   a(n) = 2^(2k - popcount(k)), where k is the unique integer with
 *            k(k+1)/2 ≤ n < (k+1)(k+2)/2 (triangular numbers)
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find k by solving k(k+1)/2 ≤ n < (k+1)(k+2)/2.  */
    /* Use double sqrt for a first estimate; adjust to avoid rounding errors. */
    long double calc = sqrt((long double)(8.0L * n + 1.0L));
    idx_t k = (idx_t)((calc - 1.0L) / 2.0L);

    /* Correct k if needed using 128‑bit arithmetic to avoid overflow. */
    __uint128_t n128 = n;
    while ( (__uint128_t)(k + 1) * (k + 2) / 2 <= n128 ) {
        k++;
    }
    while (k > 0 && (__uint128_t)k * (k + 1) / 2 > n128) {
        k--;
    }

    /* Compute exponent e = 2k - popcount(k). */
    idx_t pc = __builtin_popcountll((unsigned long long)k);
    idx_t e  = 2 * k - pc;

    /* Return 2^e.  pow2 handles any seq_t size. */
    return pow2(e);
}
