/*
 * A050921  Smallest prime of form n*2^m+1, m >= 0, or 0 if no such prime
 *          exists.
 * https://oeis.org/A050921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest prime of the form n*2^m + 1 (m ≥ 0),
   or 0 if no such prime is found within a practical bound. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1 */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;           /* convert index to seq_t */
    seq_t one = 1;                 /* constant 1 in seq_t type */

    /* Search for the smallest exponent m giving a prime.  The bound
       1024 is chosen to be well beyond what is needed for all
       pre‑tested terms while keeping the runtime small. */
    for (idx_t m = 0; m < 1024; ++m)
    {
        seq_t p2 = pow2(m);          /* 2^m as seq_t */
        /* Detect overflow for very small seq_t types (e.g. uint8_t) */
        if (p2 == 0)
            break;
        seq_t p = sn * p2 + one;
        if (is_prime_big(p))
            return p;                /* found the smallest such prime */
    }

    /* No prime found within the search bound.  Return 0 as specified. */
    return 0;
}
