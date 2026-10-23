/*
 * A183003  a(n) = A183002(n)/2.
 * https://oeis.org/A183003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * Sum_{k=1..n} (tau(k)-2 + (tau(k) mod 2))
 * Range:     u8 n <= 90, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(k) = number of positive divisors of k */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* divisor sieve: div[i] = number of divisors of i */
    idx_t m = n;
    uint32_t *div = (uint32_t *)calloc((size_t)(m + 1), sizeof(uint32_t));
    if (!div) return 0;  /* allocation failed; return 0 as a safe value */

    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t j = i; j <= m; j += i) {
            ++div[j];
        }
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k) {
        int tau = (int)div[k];          /* tau <= m, fits in int */
        int f;                          /* contribution of k */
        if (tau == 1) {
            f = 0;                      /* special case: tau(1) = 1 */
        } else {
            f = tau - 2 + (tau & 1);    /* tau%2 == tau&1 */
        }
        sum += (seq_t)f;
    }

    free(div);
    return sum / 2;
}
