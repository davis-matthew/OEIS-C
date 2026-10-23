/*
 * A008290  Triangle T(n,k) of rencontres numbers (number of permutations of n
 *          elements with k fixed points).
 * https://oeis.org/A008290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) = binomial(m,k) * A000166(m-k), * where m is the largest
 *            integer with m*(m+1)/2 <= n and * k = n - m*(m+1)/2. * This
 *            yields the flattened triangle of rencontres numbers
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 104, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* --- determine the row (m) and column (k) in the triangle --- */
    idx_t m = 0;
    while (1) {
        idx_t next = m + 1;
        idx_t next_base = next * (next + 1) / 2;
        if (next_base > n)
            break;
        m = next;
    }
    idx_t base = m * (m + 1) / 2;
    idx_t k = n - base;

    /* --- ensure derangement array D[0..m] is computed --- */
    static seq_t *D = NULL;
    static size_t dcap = 0;
    if (dcap <= (size_t)m) {
        seq_t *tmp = (seq_t *)realloc(D, (size_t)(m + 1) * sizeof(seq_t));
        if (!tmp)
            return 0;   /* allocation failure – unlikely for the bounds of this problem */
        D = tmp;
        for (size_t i = dcap; i <= (size_t)m; ++i) {
            if (i == 0)
                D[i] = 1;
            else if (i == 1)
                D[i] = 0;
            else {
                seq_t a_prev = D[i - 1];
                seq_t a_prev2 = D[i - 2];
                seq_t factor = (seq_t)(i - 1);
                D[i] = factor * (a_prev + a_prev2);
            }
        }
        dcap = m + 1;
    }

    /* --- compute the answer as product of binomial and derangement --- */
    seq_t bin = binom(m, k);
    seq_t res = bin * D[(size_t)(m - k)];
    return res;
}
