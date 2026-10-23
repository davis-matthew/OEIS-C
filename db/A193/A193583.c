/*
 * A193583  Number of fixed points under iteration of sum of squares of digits
 *          in base b.
 * https://oeis.org/A193583
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = d(n^2 + 1) - 1 * * where d(m) is the number of positive
 *            divisors of m. * For base b, the fixed points are the numbers x
 *            that satisfy * S(x) = x under the sum‑of‑squares‑of‑digits map.
 *            It can be shown * that the number of such fixed points equals
 *            the number of ways * to write (1 + b^2) as a...
 * Range:     u8 n <= 15, u16 n <= 566, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* defined only for n >= 2 */

    /* compute m = n^2 + 1 using seq_t */
    seq_t m = (seq_t)n * (seq_t)n + 1;
    seq_t divcnt = 1;              /* 1 corresponds to empty factor list */

    /* factorisation by trial division */
    for (seq_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;

        /* count exponent of prime p */
        unsigned exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        divcnt *= (seq_t)(exp + 1);
    }

    /* remaining prime factor > 1 */
    if (m > 1) {
        divcnt *= 2;   /* exponent 1 contributes factor (1+1) */
    }

    /* subtract the trivial representation (the number itself) */
    return divcnt - 1;
}
