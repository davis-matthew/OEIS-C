/*
 * A139855  Primes of the form 4x^2+4xy+31y^2.
 * https://oeis.org/A139855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime of the form 4x^2+4xy+31y^2 (OEIS A139855)
 * Range:     u8 n <= 4, u16 n <= 411, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Returns the nth element of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but harmless */

    /* static state to remember already found terms across calls */
    static seq_t *list   = NULL;   /* array holding the primes found so far */
    static size_t cap    = 0;      /* allocated capacity of the array       */
    static idx_t  count  = 0;      /* number of primes stored               */

    /* next candidates of the two residue classes */
    static seq_t next31 = (seq_t)31;   /* numbers ≡ 31 (mod 120) */
    static seq_t next79 = (seq_t)79;   /* numbers ≡ 79 (mod 120) */

    /* ensure we have at least n terms precomputed */
    while (count < n)
    {
        /* pick the smaller candidate from the two progressions */
        seq_t cand = (next31 <= next79) ? next31 : next79;

        /* test primality */
        if (is_prime_big(cand))
        {
            /* grow storage if needed */
            if ((size_t)count >= cap)
            {
                size_t newcap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t*)realloc(list, newcap * sizeof(seq_t));
                if (tmp == NULL) /* allocation failed – abort gracefully */
                    return 0;
                list = tmp;
                cap  = newcap;
            }
            list[count++] = cand;
        }

        /* advance the progression that supplied the candidate */
        if (cand == next31)
            next31 += (seq_t)120;
        else
            next79 += (seq_t)120;
    }

    return list[n-1];
}
