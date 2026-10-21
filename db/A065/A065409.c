/*
 * A065409  Fennessey-Larcombe-French sequence.
 * https://oeis.org/A065409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   * a(n)*(n^3 - n^2) = a(n-1)*(8 - 32*n^2 + 24*n^3) +
 *            a(n-2)*(256*n^2 - 128*n^3) * Initial values: a(0) = 1, a(1) = 8
 *            * * The function implements this recurrence. For C++ builds it *
 *            relies on the built‑in arithmetic of seq_t (supporting *
 *            arbitrary precision when seq_t is a large integer class). *...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifdef __cplusplus
/* C++ version – use seq_t arithmetic directly (works for native or big‑int seq_t) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)8;   /* a(1) */
    seq_t a_curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t i_s  = (seq_t)i;
        seq_t i2   = i_s * i_s;          /* i^2 */
        seq_t i3   = i2 * i_s;           /* i^3 */

        seq_t coeff1 = (seq_t)8 - (seq_t)32 * i2 + (seq_t)24 * i3;
        seq_t coeff2 = (seq_t)256 * i2 - (seq_t)128 * i3;

        seq_t num = a_prev1 * coeff1 + a_prev2 * coeff2;
        seq_t den = i3 - i2;                 /* n^3 - n^2 = n^2*(n-1) */

        a_curr = num / den;                  /* exact division */

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
#else
/* C version – use __int128 for intermediate to protect 64‑bit seq_t */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a_prev2 = (seq_t)1;
    seq_t a_prev1 = (seq_t)8;
    seq_t a_curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        __int128 ii  = i;
        __int128 i2  = ii * ii;           /* i^2   */
        __int128 i3  = i2 * ii;           /* i^3   */

        __int128 coeff1 = (__int128)8 - (__int128)32 * i2 + (__int128)24 * i3;
        __int128 coeff2 = (__int128)256 * i2 - (__int128)128 * i3;

        __int128 num = (__int128)a_prev1 * coeff1 + (__int128)a_prev2 * coeff2;
        __int128 den = i3 - i2;           /* n^3 - n^2 */

        a_curr = (seq_t)(num / den);      /* exact division */

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
#endif
