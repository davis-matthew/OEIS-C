/*
 * A129699  Least nonnegative m such that P(n+3,n) + P(n+3,m) is prime where
 *          P(k,n) is n-th k-gonal number, or -1 if no such value exists.
 * https://oeis.org/A129699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 32, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A129699:
   a(n) = least m ≥ 0 such that the sum of the n‑th and m‑th (n+3)-gonal
   numbers is prime; return -1 if no such m exists. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Polygonal numbers:  P(k,n) = ((k-2)n^2 – (k-4)n)/2  (k ≥ 3, n ≥ 0) */
static seq_t poly(seq_t k, idx_t n)
{
    seq_t nn   = (seq_t)n * (seq_t)n;                          /* n² */
    seq_t num  = (seq_t)(k - 2) * nn - (seq_t)(k - 4) * (seq_t)n; /* numerator */
    return num / 2;                                            /* division by 2 */
}

seq_t a(idx_t n)
{
    seq_t k     = (seq_t)(n + 3);          /* polygonal parameter */
    seq_t base  = poly(k, n);              /* P(k,n) */

    /* Search limit – large enough for all n up to about 10 000. */
    const idx_t MAX_M = 1000000;

    for (idx_t m = 0; m <= MAX_M; ++m)
    {
        seq_t sum = base + poly(k, m);
        if (is_prime_big(sum))
            return (seq_t)m;
    }
    /* No suitable m found within the bound. */
    return (seq_t)(-1);
}
