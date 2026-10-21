/*
 * A079813  n 0's followed by n 1's.
 * https://oeis.org/A079813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is not a perfect square and n - floor(sqrt(n))^2
 *            <= floor(sqrt(n)), else 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for idx_t (uint64_t).  */
static idx_t int_sqrt(idx_t n)
{
    /* Handle small n directly. */
    if (n < 2)
        return n;

    /* Newton's method – converges in a few iterations. */
    idx_t x = n;
    idx_t y = (x + 1) >> 1;          /* (x + 1)/2  */

    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;       /* (x + n/x)/2 */
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t k   = int_sqrt(n);      /* floor(sqrt(n))         */
    idx_t kk  = k * k;            /* k^2                    */
    if (n == kk)
        return (seq_t)0;          /* perfect square → 0     */

    /* n - k^2 <= k defines the closer‑to‑lower‑square range */
    if (n - kk <= k)
        return (seq_t)1;
    return (seq_t)0;
}
