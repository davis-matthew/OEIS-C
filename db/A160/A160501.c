/*
 * A160501  (n+1)^prime(n+1) + n^prime(n).
 * https://oeis.org/A160501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^prime(n+1) + n^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* not needed for offset 1, but defined for completeness */

    seq_t nseq  = (seq_t)n;        /* n as seq_t */
    seq_t nplus = nseq + 1;        /* n+1 as seq_t */

    /* primes are 1‑based: prime(1)=2, prime(2)=3, … */
    idx_t p_n   = get_prime(n);     /* prime(n)   */
    idx_t p_np1 = get_prime(n + 1); /* prime(n+1) */

    seq_t term1 = big_pow(nplus, (seq_t)p_np1); /* (n+1)^prime(n+1) */
    seq_t term2 = big_pow(nseq,   (seq_t)p_n);   /* n^prime(n)       */

    return term1 + term2;
}
