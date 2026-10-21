/*
 * A074754  Number of integers k such that sigma(k) divides n.
 * https://oeis.org/A074754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k ≥ 1 : sigma(k) divides n}|
 * Range:     u8 n <= 8639, u16 n <= 9997, u32 n <= 9997, u64 n <= 9997,
 *            u128 n <= 9997, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   sigma(k) = sum of positive divisors of k. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate and zero‑initialize the array of sigma values. */
    uint64_t *sigma = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    if (!sigma)
        return 0;   /* fallback on allocation failure */

    /* Divisor sieve: for each i add i to all multiples of i. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += (uint64_t)i;
    }

    /* Count k such that sigma(k) divides n. */
    seq_t cnt = 0;
    uint64_t un = (uint64_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t sk = sigma[k];
        if (sk != 0 && (un % sk == 0))
            ++cnt;
    }

    free(sigma);
    return cnt;
}
