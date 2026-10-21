/*
 * A205130  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=2*j^2-j, the j-th hexagonal number.
 * https://oeis.org/A205130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) is the least k>0 such that s(k)≡s(j) (mod n) for some 1 ≤ j < k,
 * where s(k)=2*k^2-k is the k‑th hexagonal number.
 */

#include <stdlib.h>   /* calloc, free */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, unused for n≥1 */
    /* array of bool visited[0..n-1] */
    uint8_t *seen = (uint8_t *)calloc((size_t)n, 1);
    if (!seen) return (seq_t)0;           /* allocation failure */

    seq_t n_seq = (seq_t)n;
    idx_t k;
    for (k = 1; k <= n + 1; ++k)          /* pigeonhole guarantees a hit */
    {
        seq_t kk = (seq_t)k;
        seq_t r = (2 * kk * kk - kk) % n_seq;
        size_t idx = (size_t)r;           /* r < n, fits in size_t */
        if (seen[idx]) {                  /* remainder seen before */
            free(seen);
            return (seq_t)k;
        }
        seen[idx] = 1;
    }
    /* should never reach here */
    free(seen);
    return (seq_t)(n + 1);
}
