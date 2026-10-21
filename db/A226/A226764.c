/*
 * A226764  Least k such that 1 + 1/2 + ... + 1/k < 1/(k+1) + ... + 1/n.
 * https://oeis.org/A226764
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest k such that 2*H_k ≤ H_n, where H_k = 1 + 1/2
 *            + … + 1/k. This is equivalent to a(n) = Sum_{k≥1} sign(1 -
 *            sign(2*H_k - H_n)), the OEIS definition
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 4)
        return 0;               /* behaviour below offset is irrelevant */

    /* compute H_n */
    long double Hn = 0.0L;
    for (idx_t i = 1; i <= n; ++i)
        Hn += 1.0L / (long double)i;

    /* find largest k with 2*H_k ≤ H_n */
    const long double eps = 1e-18L;   /* safety margin for rounding */
    long double Hk = 0.0L;
    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k) {
        Hk += 1.0L / (long double)k;
        if (2.0L * Hk <= Hn + eps)
            ++count;
        else
            break;
    }

    return count;
}
