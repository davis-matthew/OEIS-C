/*
 * A014687  In sequence of odd primes add 1 to first prime, 3rd prime, 5th
 *          prime, ... then subtract 1 from 2nd prime, fourth prime, sixth
 *          prime and so on.
 * https://oeis.org/A014687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+1) + (-1)^(n+1) (OEIS A014687)
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the (n+1)-st prime (1‑indexed). */
    seq_t p = (seq_t)get_prime(n + 1);

    /* Apply ±1 according to the parity of (n+1). */
    if ((n + 1) & 1U)       /* (n+1) odd → subtract 1 */
        p -= 1;
    else                    /* (n+1) even → add 1 */
        p += 1;

    return p;
}
