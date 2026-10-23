/*
 * A162530  Numbers k whose largest divisor <= sqrt(k) equals 10.
 * https://oeis.org/A162530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10 * prime(n-3) for n > 12, otherwise a pre‑computed
 *            value
 * Range:     u8 n <= 12, u16 n <= 850, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Precomputed first 12 terms */
    static const seq_t first[12] = {
        100, 110, 120, 130, 140, 150, 160, 170, 190, 200, 230, 250
    };

    if (n <= 12)
        return first[n - 1U];

    /* For n > 12: a(n) = 10 * prime(n-3) */
    seq_t p = (seq_t)get_prime(n - 3U);
    return (seq_t)10U * p;
}
