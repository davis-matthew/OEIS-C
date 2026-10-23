/*
 * A014062  a(n) = binomial(n^2, n).
 * https://oeis.org/A014062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n^2, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute N = n^2.  For the given limits (n ≤ 10 000) this fits into idx_t. */
    idx_t N = n * n;

    /* Choose the smaller of k = n and N‑k to minimise the number of iterations. */
    idx_t k = n;
    if (k > N - k) {
        k = N - k;
    }

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i) {
        /* At step i multiply by (N - k + i) and divide by i.
           Use 128‑bit arithmetic for built‑in integer types to avoid overflow
           on intermediate products.  For arbitrary‑precision seq_t the product
           naturally has sufficient range. */
        idx_t numerator = N - k + i;

        if (sizeof(seq_t) <= 8) {        /* Built‑in 8‑byte integer */
            unsigned __int128 prod = (unsigned __int128)result * (unsigned __int128)numerator;
            result = (seq_t)(prod / i);
        } else {                         /* seq_t is a big‑int type */
            seq_t prod = result * (seq_t)numerator;
            result = prod / i;           /* division is exact */
        }
    }

    return result;
}
