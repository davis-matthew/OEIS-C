/*
 * A047675  Square array a(n,k) read by antidiagonals: a(n,1)=n, a(1,k)=k,
 *          a(n,k) = a(n-1,k-1)*a(n-1,k)*a(n,k-1).
 * https://oeis.org/A047675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n,1)=n, a(1,k)=k, a(n,k)=a(n-1,k-1)*a(n-1,k)*a(n,k-1) for
 *            n,k≥2. The sequence a(n) is obtained by reading the 2‑D array by
 *            anti‑diagonals (a(1)=a(1,1), a(2)=a(2,1), a(3)=a(1,2), …)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 23, u64 n <= 30, u128 n <= 39,
 *            big any n
 * Helpers:   ij_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ij_from_n.h"

#ifndef __cplusplus
/* In pure C we cannot rely on placement new, so we use malloc for
   integer seq_t types.  */
#endif

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* Determine coordinates (i,j) of the required term. */
    idx_t target_i, target_j;
    ij_from_n(n, &target_i, &target_j);

    /* Dimension needed to hold the a(i,j). */
    idx_t needed = (target_i > target_j) ? target_i : target_j;

    /* Allocate two 1‑D arrays for the current and previous rows. */
#ifdef __cplusplus
    std::vector<seq_t> prev(needed + 1);
    std::vector<seq_t> cur (needed + 1);
#else
    seq_t *prev = (seq_t*)malloc((needed + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc((needed + 1) * sizeof(seq_t));
    if (!prev || !cur) return 0;  /* OOM – fail quietly */
#endif

    seq_t answer = 0;

    for (idx_t i = 1; i <= needed; ++i) {
        for (idx_t j = 1; j <= needed; ++j) {
            if (i == 1 && j == 1) {
                cur[j] = (seq_t)1;
            } else if (i == 1) {
                cur[j] = (seq_t)j;
            } else if (j == 1) {
                cur[j] = (seq_t)i;
            } else {
                cur[j] = prev[j-1] * prev[j] * cur[j-1];
            }
        }

        if (i == target_i) {
            answer = cur[target_j];
            break;   /* no need to compute further rows */
        }

#ifdef __cplusplus
        /* swap vectors for next iteration */
        prev.swap(cur);
#else
        /* swap pointers in C */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
#endif
    }

#ifndef __cplusplus
    /* Free memory allocated in pure C. */
    free(prev);
    free(cur);
#endif

    return answer;
}
