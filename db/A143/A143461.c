/*
 * A143461  Square array A(n,k) of numbers of length n quaternary words with
 *          at least k 0-digits between any other digits (n,k >= 0), read by
 *          antidiagonals.
 * https://oeis.org/A143461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the square array A(n,k) read by antidiagonals. A(n,0) =
 *            4^n A(n,k>0) = 3*n+1 if n ≤ k+1 = A(n-1,k) + 3*A(n-k-1,k)
 *            otherwise
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 151, u64 n <= 559,
 *            u128 n <= 2143, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute A(i,j) for i>=0, j>=0 */
static seq_t compute_A(idx_t i, idx_t j)
{
    /* column j=0: 4^i */
    if (j == 0) {
        seq_t val = 1;
        for (idx_t t = 1; t <= i; ++t)
            val *= 4;
        return val;
    }

    /* small i: direct formula */
    if (i <= j + 1)
        return (seq_t)(3 * i + 1);

    /* general case: use a circular buffer of size j+1 */
    size_t buf_size = (size_t)(j + 1);
    seq_t *buf = (seq_t *)malloc(buf_size * sizeof(seq_t));
    if (!buf)
        abort();                 /* fail if memory allocation fails */

    buf[0] = 1;                   /* A(0,j) = 1 */
    seq_t val = 0;

    for (idx_t t = 1; t <= i; ++t) {
        if (t <= j + 1) {
            val = (seq_t)(3 * t + 1);
        } else {
            size_t a = (size_t)((t - 1) % buf_size);
            size_t b = (size_t)((t - j - 1) % buf_size);
            val = buf[a] + 3 * buf[b];
        }
        buf[t % buf_size] = val;
    }

    seq_t result = buf[i % buf_size];
    free(buf);
    return result;
}

/* convert a linear index n into (row=i, col=j) along antidiagonals */
static void index_to_ij(idx_t n, idx_t *i, idx_t *j)
{
    idx_t d = 0;
    /* find largest d such that d*(d+1)/2 ≤ n */
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* starting index of diagonal d: d*(d+1)/2 */
    unsigned __int128 start = (unsigned __int128)d * (d + 1) / 2;
    idx_t pos = n - (idx_t)start;   /* offset within the diagonal */
    *i = pos;                       /* row index */
    *j = d - pos;                   /* column index */
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    idx_t i, j;
    index_to_ij(n, &i, &j);
    return compute_A(i, j);
}
