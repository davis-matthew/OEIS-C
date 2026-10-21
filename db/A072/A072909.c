/*
 * A072909  Least k>0 such that n+k is squarefree.
 * https://oeis.org/A072909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k > 0 such that n + k is squarefree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is defined for n >= 1.  For n = 0 a dummy value is
   returned; the behaviour below the offset is irrelevant. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* If n is 0 (outside the defined offset) return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Try successive k = 1, 2, ... until n + k is squarefree. */
    idx_t k = 1;
    while (1) {
        /* Compute n + k as a uint64_t value. */
        uint64_t val = (uint64_t)(n + k);

        /* Check if val is squarefree: no prime squared divides val. */
        bool squarefree = true;
        uint64_t p = 2;
        while (p <= val / p) {         /* safe: p*p <= val */
            uint64_t psq = p * p;
            if (val % psq == 0) {      /* p^2 | val → not squarefree */
                squarefree = false;
                break;
            }
            /* Continue with next prime candidate: 2,3,5,7,... */
            p = (p == 2) ? 3 : p + 2;
        }

        if (squarefree) {
            return (seq_t)k;          /* a(n) = k */
        }
        /* Next candidate k */
        ++k;
    }
}
