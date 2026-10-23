/*
 * A138459  a(n) = ((n-th prime)^6-(n-th prime)^4)/12.
 * https://oeis.org/A138459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^6-(p_n)^4)/12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, guard against n == 0 */

    uint32_t p32 = get_prime(n);    /* n‑th prime, 1‑based */
    seq_t p = (seq_t)p32;

    seq_t p2 = p * p;                /* p² */
    seq_t p4 = p2 * p2;              /* p⁴ */
    seq_t p6 = p4 * p2;              /* p⁶ */

    return (p6 - p4) / 12;           /* exact division */
}
