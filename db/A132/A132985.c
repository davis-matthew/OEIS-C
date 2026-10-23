/*
 * A132985  Expansion of chi(-q^5) / chi(-q)^5 in powers of q where chi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A132985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 47, u64 n <= 161, u128 n <= 581,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in Prod_{k>=1} (1 + x^k)^5 / (1 + x^(5k))
 * This computes the coefficients by multiplying the series by
 * (1 + x^k)^5 and dividing by (1 + x^(5k)) for each k = 1..n.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mul_small.h"

static const seq_t C5[6] = { (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)10,
                             (seq_t)5, (seq_t)1 };

seq_t a(idx_t n)
{
    /* Allocate temporary coefficient arrays */
    idx_t N = n + 1;
    seq_t *A = (seq_t *)malloc(N * sizeof(seq_t));
    seq_t *B = (seq_t *)malloc(N * sizeof(seq_t));
    if (!A || !B) return 0;    /* allocation failure (unlikely) */

    /* initialise A[0] = 1, the rest 0 */
    memset(A, 0, N * sizeof(seq_t));
    A[0] = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* clear B */
        memset(B, 0, N * sizeof(seq_t));

        /* multiply by (1 + x^k)^5 */
        for (idx_t i = 0; i < N; ++i) {
            seq_t ai = A[i];
            if (ai == 0) continue;
            for (int s = 0; s <= 5; ++s) {
                idx_t dest = i + k * (idx_t)s;
                if (dest >= N) break;
                B[dest] = B[dest] + mul_small(ai, (unsigned int)C5[s]);
            }
        }

        /* divide by (1 + x^(5k)) using recurrence B[i] = A[i] + A[i-5k]  */
        idx_t d = 5 * k;
        for (idx_t i = 0; i < N; ++i) {
            if (i < d)
                A[i] = B[i];
            else
                A[i] = B[i] - A[i - d];
        }
    }

    seq_t result = A[n];
    free(A);
    free(B);
    return result;
}
