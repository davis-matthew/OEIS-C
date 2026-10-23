/*
 * A138438  a(n) = ((n-th prime)^5-(n-th prime)^3)/24.
 * https://oeis.org/A138438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^5 - (p_n)^3) / 24
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 37, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where p_n is the n-th prime (1‑based)   */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n‑th prime as a 64‑bit value */
    uint64_t p = (uint64_t)get_prime(n);

    /* Use 128‑bit arithmetic if the compiler supports it,
       otherwise fall back to seq_t multiplication.       */
#ifdef __SIZEOF_INT128__
    unsigned __int128 mp   = (unsigned __int128)p;
    unsigned __int128 p3   = mp * mp * mp;      /* mp^3 */
    unsigned __int128 p5   = p3 * mp * mp;      /* mp^5 */
    unsigned __int128 val  = (p5 - p3) / 24;     /* (mp^5 - mp^3)/24 */
    return (seq_t)val;
#else
    /* In compilers without __int128, let seq_t handle the arithmetic. */
    seq_t sp  = (seq_t)p;
    seq_t sp3 = sp * sp * sp;                /* sp^3 */
    seq_t sp5 = sp3 * sp * sp;               /* sp^5 */
    seq_t val = (sp5 - sp3) / 24;            /* (sp^5 - sp^3)/24 */
    return val;
#endif
}
