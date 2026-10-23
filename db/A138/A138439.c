/*
 * A138439  a(n) = (prime(n)^5 - prime(n)^4)/2.
 * https://oeis.org/A138439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^5 - prime(n)^4)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* off‑offset behavior is irrelevant */
        return 0;

    /* Retrieve the n-th prime (1‑based).  The utility handles caching. */
    uint32_t p = get_prime(n);

    /* Compute p^4  and  p^5, then the requested expression. */
    seq_t ps = (seq_t)p;
    seq_t p2 = ps * ps;      /* p^2 */
    seq_t p3 = p2 * ps;      /* p^3 */
    seq_t p4 = p3 * ps;      /* p^4 */
    seq_t p5 = p4 * ps;      /* p^5 */

    seq_t result = (p5 - p4) / 2;  /* (p^5 - p^4)/2 */
    return result;
}
