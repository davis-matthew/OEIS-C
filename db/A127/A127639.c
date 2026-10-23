/*
 * A127639  A051731 * A127640, where A127640 = infinite lower triangular
 *          matrix with the sequence of primes in the main diagonal and the
 *          rest zeros.
 * https://oeis.org/A127639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS Formula (derived):
 * a(n) = p_k (the k-th prime) if there exist integers j ≥ 0 and k ≥ 1
 * such that n = k + T_j (where T_j = j(j+1)/2 is the j‑th triangular number)
 * and k divides (j+1).  
 * If no such k exists, a(n) = 0.
 *
 * Example: For n = 8, j = 3, T_j = 6, k = 2 (since 8 = 2 + 6),
 * and 2 | (3+1) = 4 → a(8) = p_2 = 3.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* iterate over triangular numbers T_j up to n-1 */
    for (idx_t j = 0; ; ++j)
    {
        idx_t tri = j * (j + 1) / 2;     /* T_j */
        if (tri > n - 1)
            break;                      /* no larger j will satisfy n-k = T_j */

        idx_t k = n - tri;              /* candidate prime index */
        if (k == 0)
            continue;                   /* skip, k must be ≥ 1 */

        /* check divisibility condition: k | (j+1)  ⇔  (j+1) % k == 0 */
        if (((j + 1) % k) == 0)
            return (seq_t)get_prime(k);
    }

    /* no suitable k found → result is 0 */
    return 0;
}
