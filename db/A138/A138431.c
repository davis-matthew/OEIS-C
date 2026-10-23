/*
 * A138431  a(n) = ((n-th prime)^5-(n-th prime)^2)/2.
 * https://oeis.org/A138431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n))^5-(prime(n))^2)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 25, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)                /* out of range, but not needed for OEIS offset */
        return 0;

    uint32_t p = get_prime(n); /* nth prime */
    if (p == 0)                /* defensive: get_prime may return 0 if out of limits */
        return 0;

    /* Compute ((p^5 - p^2)/2) with sufficient intermediate precision. */
    if (sizeof(seq_t) <= 8) {
        /* use 128‑bit integer when seq_t is 8, 16, 32 or 64 bits */
        unsigned __int128 pp = p;
        unsigned __int128 p2 = pp * pp;          /* p^2 */
        unsigned __int128 p5 = p2 * p2 * pp;      /* p^4 * p = p^5 */
        unsigned __int128 num = p5 - p2;          /* p^5 - p^2 */
        return (seq_t)(num / 2);
    } else {
        /* seq_t is 128 bits or arbitrary precision – normal arithmetic suffices */
        seq_t ps = (seq_t)p;
        seq_t p2 = ps * ps;
        seq_t p5 = p2 * p2 * ps;
        seq_t num = p5 - p2;
        return num / 2;
    }
}
