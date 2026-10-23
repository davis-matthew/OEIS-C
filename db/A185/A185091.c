/*
 * A185091  The smallest positive noncomposite q such that 2n-1 = 2p+q for
 *          some positive noncomposite p.
 * https://oeis.org/A185091
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 784, u16 n <= 9284, u32 n <= 9284, u64 n <= 9284,
 *            u128 n <= 9284, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A185091.
   a(n) is the smallest positive non‑composite q such that
   2n–1 = 2p + q with p, q non‑composite (1 or prime).  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; for smaller n the
       returned value is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /*  N = 2n – 1  (odd).  Cast n to seq_t first to avoid overflow
        in the intermediate product. */
    seq_t ns = (seq_t)n;
    seq_t N  = (seq_t)2 * ns - (seq_t)1;

    /* Try q = 1 first.  It works iff p = (N–1)/2 is 1 or prime. */
    seq_t p0 = (N - (seq_t)1) / (seq_t)2;
    if (p0 == (seq_t)1 || isPrimeGeneric(p0))
        return (seq_t)1;

    /* Scan odd values for q, testing primality on the fly. */
    for (seq_t q = (seq_t)3; q < N; q += (seq_t)2)
    {
        if (!isPrimeGeneric(q))
            continue;

        seq_t p = (N - q) / (seq_t)2;
        if (p == (seq_t)0) /* p would become < 1 */
            break;

        if (p == (seq_t)1 || isPrimeGeneric(p))
            return q;
    }

    /* By definition an appropriate q always exists; this is a safe
       fallback that should never be reached. */
    return (seq_t)1;
}
