/*
 * A052216  Sums of two powers of 10.
 * https://oeis.org/A052216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 10^i + 10^j, where i ≥ j ≥ 0 are chosen in increasing
 *            order of i and, for each i, increasing order of j. This
 *            enumerates all integers that are sums of two powers of 10; the
 *            n-th term (with offset 1) is a(n)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 55, u64 n <= 209, u128 n <= 780,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Find the exponent i such that the cumulative count up to i
       is at least n.  The cumulative count up to exponent i-1 is
       i*(i+1)/2, and after adding exponent i we have
       (i+1)*(i+2)/2 terms. */
    idx_t i = 0;
    idx_t cumulative = 1;          /* terms for i = 0 (just the number 2) */

    while (cumulative < n) {
        ++i;
        cumulative += i + 1;       /* add the (i+1) terms for the new exponent i */
    }

    idx_t prev = cumulative - (i + 1);   /* counts up to exponent i-1 */
    idx_t j = n - prev - 1;              /* zero‑based offset within exponent i */

    return pow10(i) + pow10(j);
}
