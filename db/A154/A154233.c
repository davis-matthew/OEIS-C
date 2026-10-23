/*
 * A154233  Triangle T(n, k) = T(n-1, k) + T(n-1, k-1) + (n^2 +n -1)*T(n-2,
 *          k-1), read by rows.
 * https://oeis.org/A154233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangular array T(n,k) = T(n-1,k) +
 *            T(n-1,k-1) + (n^2 + n - 1) * T(n-2,k-1), with T(n,0) = T(n,n) =
 *            1 and the triangle read by rows. a(0) = 1, a(1) = 1, a(2) = 1, …
 *            (OEIS A154233)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 82, u64 n <= 218,
 *            u128 n <= 575, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find the row r and position k within that row */
    idx_t r = 0;
    while ((seq_t)(r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - (idx_t)((seq_t)r * (r + 1) / 2);

    /* trivial case */
    if (r == 0)
        return (seq_t)1;

    /* ensure we have enough space for rows up to r */
    static seq_t *prev2 = NULL;      /* row n-2 */
    static seq_t *prev1 = NULL;      /* row n-1 */
    static seq_t *cur   = NULL;      /* current row */
    static size_t buf_size = 0;      /* allocated size of the above arrays */

    size_t needed = (size_t)(r + 1);
    if (buf_size < needed) {
        size_t new_size = needed;
        seq_t *p2 = (seq_t *)realloc(prev2, new_size * sizeof(seq_t));
        seq_t *p1 = (seq_t *)realloc(prev1, new_size * sizeof(seq_t));
        seq_t *pc = (seq_t *)realloc(cur,   new_size * sizeof(seq_t));
        prev2 = p2; prev1 = p1; cur = pc;
        buf_size = new_size;
    }

    /* row 0 */
    prev2[0] = (seq_t)1;

    /* row 1 */
    prev1[0] = (seq_t)1;
    prev1[1] = (seq_t)1;

    /* build rows 2 … r */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t factor = (seq_t)i * (seq_t)i + (seq_t)i - (seq_t)1;

        cur[0] = (seq_t)1;
        cur[i] = (seq_t)1;

        for (idx_t j = 1; j < i; ++j) {
            cur[j] = prev1[j] + prev1[j - 1] + factor * prev2[j - 1];
        }

        /* rotate buffers */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }

    return prev1[k];
}
