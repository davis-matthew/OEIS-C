/*
 * A132657  a(n) is the product of the least prime > n^2 and the greatest
 *          prime < (n+1)^2.
 * https://oeis.org/A132657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (least prime > n^2) * (greatest prime < (n+1)^2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                   /* undefined for n < 1 */

    /* search for the smallest prime > n^2 */
    uint64_t p1 = (uint64_t)n * (uint64_t)n + 1;
    while (!is_prime(p1))
        ++p1;

    /* search for the largest prime < (n+1)^2 */
    uint64_t p2 = (uint64_t)(n + 1) * (uint64_t)(n + 1) - 1;
    while (!is_prime(p2))
        --p2;

    return (seq_t)p1 * (seq_t)p2;
}
