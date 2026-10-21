/*
 * A073637  Digital root (cf. A010888) of prime(n)^3.
 * https://oeis.org/A073637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digital root of prime(n)^3
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* obtain the nth prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* work modulo 9: the digital root of p³ depends only on p mod 9 */
    seq_t pm = (seq_t)(p % 9);          /* pm ∈ {0,…,8} */

    /* cube modulo 9 */
    seq_t cm = (pm * pm * pm) % 9;      /* cm ∈ {0,…,8} */

    /* translate the modulo‑9 result to the true digital root:
       0 indicates a multiple of 9 → digital root 9. */
    return (cm == 0) ? 9 : cm;
}
