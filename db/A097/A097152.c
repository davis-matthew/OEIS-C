/*
 * A097152  Smallest prime p greater than n for which n is a quadratic residue
 *          mod p.
 * https://oeis.org/A097152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p > n such that n is a quadratic residue
 *            modulo p
 * Range:     u8 n <= 205, u16 n <= 9744, u32 n <= 9744, u64 n <= 9744,
 *            u128 n <= 9744, big any n
 * Helpers:   jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* value below offset is irrelevant */

    uint64_t cand = n;     /* start just below the first possible candidate */
    for (;;)
    {
        cand = next_prime(cand);           /* smallest prime > current cand */
        if (jacobi_symbol((uint64_t)n, cand) == 1)
            return (seq_t)cand;            /* found the required prime */
    }
}
