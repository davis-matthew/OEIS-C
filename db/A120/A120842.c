/*
 * A120842  a(n) = the (number of positive divisors of n)th positive integer
 *          which is coprime to n.
 * https://oeis.org/A120842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the d(n)-th positive integer coprime to n, * where d(n) =
 *            number of positive divisors of n. Computed by a simple * linear
 *            search
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* safety for n=0 */

    uint64_t tn = (uint64_t)n;                    /* n fits in 64‑bit */
    idx_t  divcnt = divisor_count_u64(tn);        /* d(n) */

    seq_t target = (seq_t)divcnt;                /* we need this many coprimes */
    seq_t count  = (seq_t)0;                      /* how many found so far */
    seq_t cand   = (seq_t)0;                      /* candidate integer */

    for (;;)
    {
        cand++;                                  /* next positive integer */

        /* gcd(cand, n) == 1  ->  cand is coprime to n */
        if (gcd(cand, (seq_t)tn) == (seq_t)1)
        {
            count++;
            if (count == target) return cand;    /* d(n)-th coprime found */
        }
    }
}
