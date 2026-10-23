/*
 * A184544  Number of (n+2) X 7 binary arrays with each 3 X 3 subblock having
 *          rows and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (1/5040)*n^7 + (1/120)*n^6 + (53/360)*n^5 + (17/12)*n^4
 *            * + (5767/720)*n^3 + (3063/40)*n^2 + (11959/70)*n + 52 *
 *            Implemented via integer arithmetic using common denominator 5040
 *            so * that the result is exact for all n >= 1
 * Range:     u8 none, u16 n <= 10, u32 n <= 74, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Off‑by‑one: OEIS a(1) is first term.  For n <= 0 return 0. */
    if (n == 0) return (seq_t)0U;

    /* Common denominator 5040.  All divisions below are exact. */
    const unsigned __int128 LCM = 5040U;
    unsigned __int128 n128 = (unsigned __int128)n;

    /* Compute successive powers of n.  All fit in 128‑bit for n up to ~10^4. */
    unsigned __int128 n1 = n128;
    unsigned __int128 n2 = n1 * n128;
    unsigned __int128 n3 = n2 * n128;
    unsigned __int128 n4 = n3 * n128;
    unsigned __int128 n5 = n4 * n128;
    unsigned __int128 n6 = n5 * n128;
    unsigned __int128 n7 = n6 * n128;

    unsigned __int128 total = 0;

    /* Add integer numerators multiplied by (LCM / denominator). */
    total += (unsigned __int128)1  * n7 * (LCM/5040U);   /* n^7 / 5040   */
    total += (unsigned __int128)1  * n6 * (LCM/120U);    /* n^6 / 120    */
    total += (unsigned __int128)53 * n5 * (LCM/360U);    /* 53*n^5 / 360 */
    total += (unsigned __int128)17 * n4 * (LCM/12U);     /* 17*n^4 / 12  */
    total += (unsigned __int128)5767 * n3 * (LCM/720U);  /* 5767*n^3 / 720 */
    total += (unsigned __int128)3063 * n2 * (LCM/40U);   /* 3063*n^2 / 40 */
    total += (unsigned __int128)11959 * n1 * (LCM/70U);  /* 11959*n / 70  */
    total += (unsigned __int128)52 * (LCM/1U);           /* constant 52   */

    seq_t result = (seq_t)(total / LCM);
    return result;
}
