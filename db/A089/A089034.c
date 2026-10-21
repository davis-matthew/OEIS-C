/*
 * A089034  a(n) = (prime(n)^4 - 1) / 240.
 * https://oeis.org/A089034
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)^4 - 1) / 240 (offset 4)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 168, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Undefined for indices below the OEIS offset 4.  Return 0. */
    if (n < 4)
        return 0;

    /* Get the n‑th prime number. */
    uint64_t p64 = get_prime(n);
    /* Use 128‑bit arithmetic to avoid overflow of p^4. */
    unsigned __int128 pp = (unsigned __int128)p64 * p64;   /* p²  */
    unsigned __int128 p4 = pp * pp;                       /* p⁴  */

    /* Compute (p⁴‑1)/240, then cast down to the output type. */
    unsigned __int128 num = p4 - 1;
    unsigned __int128 den = 240;
    seq_t result = (seq_t)(num / den);

    return result;
}
