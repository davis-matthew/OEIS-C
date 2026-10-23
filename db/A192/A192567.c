/*
 * A192567  a(n) =
 *          sum(abs(stirling1(n+1,k+1))*stirling2(n+1,k+1)*k!^2,k=0..n).
 * https://oeis.org/A192567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} |S1(n+1,k+1)| * S2(n+1,k+1) * (k!)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* m = n + 1 is the size of the Stirling table row needed */
    idx_t mmax = n + 1;

    /* Allocate temporary arrays for Stirling numbers and factorials.
       All arrays hold seq_t objects and are zero-initialised by seq_alloc_array. */
    seq_t *s1_prev = seq_alloc_array(mmax + 1);
    seq_t *s1_cur  = seq_alloc_array(mmax + 1);
    seq_t *s2_prev = seq_alloc_array(mmax + 1);
    seq_t *s2_cur  = seq_alloc_array(mmax + 1);
    seq_t *fact    = seq_alloc_array(n + 1);

    /* Initialise factorials: fact[0] = 1 */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Initial row m = 0: S1(0,0) = S2(0,0) = 1 */
    s1_prev[0] = (seq_t)1;
    s2_prev[0] = (seq_t)1;

    /* Result variable */
    seq_t result = (seq_t)0;

    /* Build Stirling rows incrementally up to m = n + 1 */
    for (idx_t m = 1; m <= mmax; ++m) {
        s1_cur[0] = (seq_t)0;
        s2_cur[0] = (seq_t)0;

        /* Compute current row from previous one */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t part1 = s1_prev[k - 1];
            seq_t part2 = (k <= m - 1) ? ((seq_t)(m - 1) * s1_prev[k]) : 0;
            s1_cur[k] = part1 + part2;

            seq_t part3 = s2_prev[k - 1];
            seq_t part4 = (k <= m - 1) ? ((seq_t)k * s2_prev[k]) : 0;
            s2_cur[k] = part3 + part4;
        }

        /* When this row corresponds to n (i.e. m-1 == n) compute the sum */
        if (m - 1 == n) {
            result = (seq_t)0;
            for (idx_t k = 0; k <= n; ++k) {
                seq_t term = s1_cur[k + 1] * s2_cur[k + 1];
                term = term * fact[k];
                term = term * fact[k];
                result += term;
            }
        }

        /* Swap previous and current rows for next iteration */
        seq_t *tmp;
        tmp = s1_prev; s1_prev = s1_cur; s1_cur = tmp;
        tmp = s2_prev; s2_prev = s2_cur; s2_cur = tmp;
    }

    /* Free the temporary arrays */
    seq_free_array(s1_prev);
    seq_free_array(s1_cur);
    seq_free_array(s2_prev);
    seq_free_array(s2_cur);
    seq_free_array(fact);

    return result;
}
