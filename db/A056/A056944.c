/*
 * A056944  Amount by which used area of rectangle needed to enclose a
 *          non-touching spiral of length n on a square lattice exceeds unused
 *          area.
 * https://oeis.org/A056944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2n - floor((sqrt(8n+1)-1)/2)·ceil((sqrt(8n+1)-1)/2)
 *            Computed by finding the largest triangular number ≤ n. a(n) = t
 *            if n = t(t+1)/2 a(n) = 2k if n = t(t+1)/2 + k (0 < k ≤ t)
 * Range:     u8 n <= 30, u16 n <= 722, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* index converted to seq_t */
    seq_t low   = 0;
    seq_t high  = n_seq;

    /* binary search for the largest t with t(t+1)/2 ≤ n */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;          /* ceil((low+high)/2) */
        seq_t tri = mid * (mid + 1);                /* mid*(mid+1) */
        tri = tri / 2;                              /* /2 as integer division */
        if (tri <= n_seq)
            low = mid;
        else
            high = mid - 1;
    }

    seq_t t   = low;
    seq_t tri = t * (t + 1) / 2;                   /* t(t+1)/2 */
    seq_t k   = n_seq - tri;                       /* excess over the last triangular */

    if (k == 0)
        return t;                                 /* n is a triangular number */
    return 2 * k;                                 /* otherwise 2k */
}
