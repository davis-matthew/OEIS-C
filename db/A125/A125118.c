/*
 * A125118  Triangle read by rows: T(n,k) = value of the n-th repunit in base
 *          (k+1) representation, 1<=k<=n.
 * https://oeis.org/A125118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 63, u64 n <= 150,
 *            u128 n <= 403, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{i=0}^{r-1} (c+1)^i
 * where r = smallest integer with r(r+1)/2 ≥ n
 * and c = n - (r-1)r/2 (the column index, 1 ≤ c ≤ r).
 * This corresponds exactly to the definition of OEIS A125118.
 * The function works for any build where seq_t and idx_t are unsigned integer types.
 */
static inline idx_t isqrt_u64(idx_t x) {
    /* integer square root, 0 ≤ isqrt(x) ≤ 2^32-1 for 64‑bit inputs */
    idx_t low = 0, high = x;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static inline idx_t triangular_root(idx_t n) {
    /* binary search for minimal r with r(r+1)/2 >= n */
    idx_t low = 1;
    idx_t high = isqrt_u64(2 * n) + 2;   /* upper bound for the root */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* determine the row (r) and column (c) in the triangle */
    idx_t r = triangular_root(n);
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t c = n - prev_tri;                       /* 1 <= c <= r */

    /* compute the finite geometric sum Σ_{i=0}^{r-1} (c+1)^i */
    seq_t val = 0;
    seq_t pow = 1;                                /* (c+1)^0 */
    seq_t base = (seq_t)c + 1;                    /* c+1 */

    for (idx_t i = 0; i < r; ++i) {
        val += pow;
        pow *= base;                              /* unsigned overflow is defined */
    }
    return val;
}
