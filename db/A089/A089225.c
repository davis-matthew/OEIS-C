/*
 * A089225  Triangle T(n,k) read by rows, defined by T(n,k) =
 *          (n-k)*T(n-1,k)+Sum(k=1..n, T(n-1,k)); T(1,1) = 1, T(1,k)= 0 if k
 *          >1.
 * https://oeis.org/A089225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) enumerates the triangle T(n,k) defined by T(1,1)=1, T(n,k)
 *            = (n-k)*T(n‑1,k)+∑_{t=1}^{n}T(n‑1,t) (k≤n). a(n) is the n‑th
 *            element when the triangle is read by rows
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* find the row r such that r*(r+1)/2 >= n */
    size_t r = 1, cum = 1;
    while (cum < (size_t)n) {
        ++r;
        cum += r;
    }

    size_t size = r + 1;             /* indices 0..r, we use 1..r */
    seq_t *prev = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc(size * sizeof(seq_t));
    if (!prev || !curr) return 0;    /* allocation failed – fallback to 0 */

    for (size_t i = 0; i < size; ++i) prev[i] = 0;
    prev[1] = (seq_t)1;

    size_t counter = 1;
    if (counter == (size_t)n) {
        seq_t val = prev[1];
        free(prev); free(curr);
        return val;
    }

    for (size_t i = 2; i <= r; ++i) {
        seq_t sumprev = 0;
        for (size_t k = 1; k < i; ++k) sumprev += prev[k];

        for (size_t k = 1; k <= i; ++k) {
            seq_t val;
            if (k < i)
                val = (seq_t)(i - k) * prev[k] + sumprev;
            else            /* k == i */
                val = sumprev;
            ++counter;
            if (counter == (size_t)n) {
                free(prev); free(curr);
                return val;
            }
            curr[k] = val;
        }
        /* swap pointers for next iteration */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }

    free(prev); free(curr);
    return 0;   /* unreachable, but keeps compiler happy */
}
