/*
 * A138425  a(n) = (prime(n)^5 - prime(n))/3.
 * https://oeis.org/A138425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^5 - prime(n))/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 27, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We rely on the utility that returns the n‑th prime (1‑based index). */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle out of bounds gracefully – although the sequence is defined only for n ≥ 1. */
    if (n == 0)
        return (seq_t)0;

    /* Fetch the n‑th prime.  get_prime is defined in the utils library. */
    uint32_t p32 = get_prime(n);

    /* Compute the value using a 128‑bit intermediate in C.  This is sufficient
       for all cases where the result fits in the target seq_t type.  For
       arbitrary‑precision seq_t implementations, the cast to seq_t will use a
       constructor that accepts an unsigned long long, yielding the exact value. */
    unsigned __int128 val = (unsigned __int128)p32;
    val *= (unsigned __int128)p32;   /* p^2 */
    val *= (unsigned __int128)p32;   /* p^3 */
    val *= (unsigned __int128)p32;   /* p^4 */
    val *= (unsigned __int128)p32;   /* p^5 */
    val -= (unsigned __int128)p32;   /* p^5 - p */
    val /= (unsigned __int128)3;     /* divide by 3 */

    return (seq_t)val;
}
