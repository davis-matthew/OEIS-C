/*
 * A083175  Row sums in A083175.
 * https://oeis.org/A083175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * ( n*(n+1)/2 if n is odd * n*(n+2)/2 if n is
 *            even )
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is 1‑indexed; return 0 for safety */

    seq_t p  = (seq_t)get_prime(n);   /* n‑th prime */
    seq_t tri;

    if ((n & 1) == 0) {             /* even n */
        tri = (seq_t)n * ((seq_t)n + 2u) / 2u;
    } else {                        /* odd n */
        tri = (seq_t)n * ((seq_t)n + 1u) / 2u;
    }

    return p * tri;
}
