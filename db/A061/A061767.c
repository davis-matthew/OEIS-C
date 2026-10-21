/*
 * A061767  a(n) = smallest composite number of the form k*n + 1.
 * https://oeis.org/A061767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest composite number of the form k*n + 1
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)     /* for completeness – not used in OEIS offset */
        return 0;

    idx_t k = 1;
    while (1) {
        seq_t m = (seq_t)n * (seq_t)k + 1ULL;   /* compute k*n + 1 */
        /* use 64‑bit primality test for the small numbers we encounter */
        if ((uint64_t)m > 1 && !is_prime((idx_t)m))
            return m;
        ++k;
    }
}
