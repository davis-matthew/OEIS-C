/*
 * A173727  Number of reduced 3 X 3 semimagic squares with distinct
 *          nonnegative integer entries and maximum entry n.
 * https://oeis.org/A173727
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2*n^3-33*n^2+174*n-264) + 9*(n mod 2)*(n^2-16*n+50) +
 *            24*((2*n^2+1) mod 3)*(n-9) + 48*(n mod 3) - 144*[(n mod 4)=0] -
 *            144*[(n mod 5)=0]
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 900, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 8 (a(8) is the first term). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The formula is valid only for n >= 8; for safety, we clamp to 8. */
    if (n < 8) {
        /* behaviour below offset is irrelevant; we just return 0 */
        return (seq_t)0;
    }

    /* Use signed 64‑bit intermediate arithmetic to avoid underflow
     * in the subtraction terms.  All intermediate values fit in ~6e12,
     * well below 9e18 (int64_t limit).  Finally cast to seq_t. */
    int64_t N = (int64_t)n;

    int64_t term1 = 3 * (2*N*N*N - 33*N*N + 174*N - 264);

    int64_t term2 = (N % 2 != 0) ? 9 * (N*N - 16*N + 50) : 0;

    int64_t term3 = 24 * (((2*N*N + 1) % 3) * (N - 9));

    int64_t term4 = 48 * (N % 3);

    int64_t term5 = -144 * (N % 4 == 0 ? 1 : 0);
    int64_t term6 = -144 * (N % 5 == 0 ? 1 : 0);

    int64_t result = term1 + term2 + term3 + term4 + term5 + term6;

    return (seq_t)result;
}
