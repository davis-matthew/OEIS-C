/*
 * A108644  Square array A(n,k) read by ascending antidiagonals: A(n,n) = n^2,
 *          if n>k: A(n,k) = n*(n-1) + k, if k>n: A(n,k) = n + (k-1)^2.
 * https://oeis.org/A108644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array A(n,k) along
 *            ascending antidiagonals. The entry is defined by A(n,n) = n^2,
 *            if n > k : A(n,k) = n*(n-1) + k, if k > n : A(n,k) = n + (k-1)^2
 * Range:     u8 n <= 136, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The indexing of the sequence is given by:  a(1) = A(1,1),
   then a(2) = A(2,1), a(3) = A(1,2), a(4) = A(3,1), a(5) = A(2,2), a(6) = A(1,3), …   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for out‑of‑range calls */

    /* Find minimal d >= 2 such that d*(d-1)/2 >= n */
    idx_t low  = 2;
    idx_t high = 2;
    while ((__uint128_t)(high - 1) * high / 2 < n)
        high <<= 1;                  /* exponential search for an upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cnt = (idx_t)((__uint128_t)(mid - 1) * mid / 2);
        if (cnt >= n) high = mid;
        else low = mid + 1;
    }
    idx_t d = low;                    /* sum i + j of the current antidiagonal */

    idx_t prev      = (idx_t)((__uint128_t)(d - 2) * (d - 1) / 2);   /* count before this diagonal */
    idx_t offset0   = n - prev - 1;                      /* 0‑based position inside the diagonal */
    idx_t i         = d - 1 - offset0;                   /* row index */
    idx_t j         = d - i;                              /* column index */

    if (i == j) {
        return (seq_t)i * (seq_t)i;
    } else if (i > j) {
        return (seq_t)i * ((seq_t)i - 1) + (seq_t)j;
    } else { /* j > i */
        idx_t t = j - 1;
        return (seq_t)i + (seq_t)t * (seq_t)t;
    }
}
