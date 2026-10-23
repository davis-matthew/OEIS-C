/*
 * A170949  "Conway's Converger": a reordering of the integers (see Comments
 *          for definition).
 * https://oeis.org/A170949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   Conway's Converger sequence – integers reordered in blocks of
 *            odd length
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t diff = hi - lo;
        idx_t add = (diff >> 1) + (diff & 1);   /* (diff+1)/2  */
        idx_t mid = lo + add;
        if (mid > n / mid) hi = mid - 1;
        else lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* 1. Determine the block number k such that (k-1)^2 < n <= k^2  */
    idx_t k = floor_sqrt(n);
    if (k * k < n) k++;

    /* 2. Compute block parameters */
    idx_t mid   = k - 1;                    /* 0‑based centre position within the block */
    idx_t start_idx = mid * mid + 1;        /* integer value of the first element in the block  */
    idx_t offset = n - start_idx;           /* position of n within the block (0‑based)       */
    seq_t start = (seq_t)mid * (seq_t)mid + 1;

    /* 3. Return the appropriate value according to the position */
    if (offset == mid) {
        /* centre element */
        return start;
    } else if (offset < mid) {
        /* left side – odd‑numbered steps */
        idx_t t = mid - offset;            /* t = 1,2,3,...  */
        return start + (seq_t)(2 * t) - 1;
    } else {
        /* right side – even‑numbered steps */
        idx_t t = offset - mid;            /* t = 1,2,3,...  */
        return start + (seq_t)(2 * t);
    }
}
