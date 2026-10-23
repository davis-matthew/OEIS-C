/*
 * A185784  Accumulation array of A107985, by antidiagonals.
 * https://oeis.org/A185784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is the n-th element of the 2D array
 *            T(n,k) = (n + k + 1) * n*(n+1)*k*(k+1) / 12 (indices n,k start
 *            at 1). The sequence is read by antidiagonals: first the element
 *            (1,1), then (1,2),(2,1), then (1,3),(2,2),(3,1), …
 * Range:     u8 n <= 16, u16 n <= 200, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the diagonal index s such that n lies in the s-th diagonal
   (the diagonal with pairs (i,j) satisfying i+j = s+1, i,j ≥ 1).      */
static void find_diag(idx_t n, idx_t *s)
/*  The diagonal number s satisfies s*(s-1)/2 ≥ n and (s-1)*(s-2)/2 < n.  */
{
    if (n <= 1) { *s = 2; return; }

    idx_t low = 2, high = 2;
    /* expand high until it is large enough */
    while (((unsigned __int128)high * (high - 1)) / 2 < n) {
        low = high;
        high <<= 1;
        if (high == 0) {           /* overflow guard */
            high = (idx_t)-1;
            break;
        }
    }
    /* binary search between low+1 and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = ((unsigned __int128)mid * (mid - 1)) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *s = low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    idx_t s;                      /* diagonal index */
    find_diag(n, &s);

    /* previous diagonal count: (s-1)*(s-2)/2 */
    unsigned __int128 prev_cnt = ((unsigned __int128)(s - 1) * (s - 2)) / 2;
    idx_t offset = (idx_t)(n - prev_cnt - 1);   /* 0‑based position in diagonal */

    idx_t i = 1 + offset;          /* row index (n in T) */
    idx_t j = s - 1 - offset;      /* column index (k in T) */

    /* compute T(i,j) = (i + j + 1) * i*(i+1) * j*(j+1) / 12   */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;

    seq_t bin_i = ii * (ii + 1) / 2;        /* C(i+1,2) */
    seq_t bin_j = jj * (jj + 1) / 2;        /* C(j+1,2) */
    seq_t sum_ij = (seq_t)(i + j + 1);

    seq_t result = bin_i * bin_j;           /* i*(i+1)/2 * j*(j+1)/2 */
    result = result * sum_ij;              /* multiply by (i+j+1) */
    result = result / 3;                   /* divide by 3 to get final value */

    return result;
}
