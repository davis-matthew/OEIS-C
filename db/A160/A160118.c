/*
 * A160118  Number of "ON" cells at n-th stage in simple 2-dimensional
 *          cellular automaton (see Comments for precise definition).
 * https://oeis.org/A160118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1. For n>=2, if n is even: a(n) = 9 + 32 * S(n/2)
 *            if n is odd : a(n) = 9 + 4 * S((n+1)/2) + 28 * S((n-1)/2) where
 *            S(m) = sum_{k=2..m} A147610(k) and A147610(2)=1, A147610(2k) = 3
 *            * A147610(k), A147610(2k+1) = A147610(k+1)
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* maximum k needed for computing A147610: ceil(n/2) */
    idx_t max_k = (n + 1) / 2;
    if (max_k < 2) max_k = 2;          /* ensure at least indices 0..2 */

    /* allocate arrays: A[0..max_k], prefix[0..max_k] */
    seq_t *A      = (seq_t *)malloc((max_k + 1) * sizeof(seq_t));
    seq_t *prefix = (seq_t *)malloc((max_k + 1) * sizeof(seq_t));
    if (!A || !prefix) {              /* safety on failure */
        free(A); free(prefix);
        return (seq_t)0;
    }

    /* initialise first values of A */
    A[0] = (seq_t)0;
    A[1] = (seq_t)0;
    A[2] = (seq_t)1;

    /* compute A[3..max_k] */
    for (idx_t k = 3; k <= max_k; ++k) {
        if ((k & 1) == 0) {                 /* even k */
            A[k] = (seq_t)3 * A[k >> 1];
        } else {                            /* odd k */
            A[k] = A[(k + 1) >> 1];
        }
    }

    /* prefix sums: prefix[m] = sum_{k=2..m} A[k] */
    prefix[0] = (seq_t)0;
    prefix[1] = (seq_t)0;
    prefix[2] = A[2];
    for (idx_t k = 3; k <= max_k; ++k) {
        prefix[k] = prefix[k - 1] + A[k];
    }

    seq_t result;
    if ((n & 1) == 0) {                     /* n even */
        idx_t m = n >> 1;
        /* m will be <= max_k because max_k >= (n+1)/2 >= n/2 */
        result = (seq_t)9 + (seq_t)32 * prefix[m];
    } else {                                /* n odd */
        idx_t m = (n + 1) >> 1;              /* m = (n+1)/2 */
        /* for n odd, m-1 <= max_k */
        result = (seq_t)9 + (seq_t)4 * prefix[m] + (seq_t)28 * prefix[m - 1];
    }

    free(A);
    free(prefix);
    return result;
}
