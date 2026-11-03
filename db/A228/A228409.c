/*
 * A228409  a(n) = 4*mu(n) + 5, where mu is the Moebius function (A008683).
 * https://oeis.org/A228409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*mu(n) + 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8595 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset starts at 1; undefined for 0 */
        return (seq_t)0;

    int mu = mu_of(n);   /* mu_of returns 1, 0, or -1 */

    if (mu == 0)
        return (seq_t)5;
    if (mu == 1)
        return (seq_t)9;

    /* mu == -1 */
    return (seq_t)1;
}
