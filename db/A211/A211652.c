/*
 * A211652  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^4<x^4+y^4.
 * https://oeis.org/A211652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with w,x,y∈{1,…,n} and
 *            w^4 < x^4 + y^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Precompute fourth powers p[i] = i^4 for 1 ≤ i ≤ n */
    seq_t *p = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!p) return 0; /* allocation failure, return 0 as fallback */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = (seq_t)i;
        p[i] = t * t * t * t;
    }

    seq_t result = 0;
    seq_t total_unordered = (seq_t)n * ((seq_t)n + 1) / 2;

    for (idx_t w = 1; w <= n; ++w) {
        seq_t T = p[w];

        /* Count unordered pairs (i,j) with i ≤ j and p[i]+p[j] ≤ T */
        idx_t i = 1, j = n;
        seq_t unordered_leT = 0;

        while (i <= j) {
            while (i <= j && p[i] + p[j] > T) j--;
            if (i > j) break;
            unordered_leT += (seq_t)(j - i + 1);
            i++;
        }

        /* Count diagonal pairs with 2*p[i] > T */
        seq_t half = T / 2;          /* floor division */
        idx_t low = 1, high = n;
        idx_t pos = n + 1;           /* position of first element > half */
        while (low <= high) {
            idx_t mid = (low + high) / 2;
            if (p[mid] > half) {
                pos = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        seq_t D_exceed = (pos <= n) ? (seq_t)(n - pos + 1) : 0;

        seq_t unordered_exceed = total_unordered - unordered_leT;
        seq_t over_win = 2 * unordered_exceed - D_exceed;
        result += over_win;
    }

    free(p);
    return result;
}
