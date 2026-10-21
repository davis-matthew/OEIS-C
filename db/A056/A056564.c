/*
 * A056564  Number of times primes can be produced by taking the absolute
 *          difference between the n-th triangular number and another
 *          triangular number.
 * https://oeis.org/A056564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of primes produced by |T_n – T_k| where T_k ≤ 2T_n
 *            + 3. T_n is the n‑th triangular number T_n = n(n+1)/2
 * Range:     u8 n <= 15, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* triangular number of index n */
    seq_t m = (seq_t)n * ((seq_t)n + 1) / 2;
    /* upper bound for k: largest T_k with T_k ≤ 2*m + 3 */
    seq_t threshold = 2 * m + 3;

    seq_t count = 0;
    seq_t t = 0;            /* T_0 */

    for (idx_t k = 0; ; ++k)
    {
        /* absolute difference between T_n and T_k */
        seq_t diff = (m > t) ? (m - t) : (t - m);
        if (isPrimeGeneric(diff))
            count += 1;

        /* compute next triangular number T_{k+1} = T_k + (k+1) */
        seq_t next_t = t + (seq_t)(k + 1);
        if (next_t > threshold)
            break;
        t = next_t;
    }
    return count;
}
