/*
 * A014200  Number of solutions to x^2 + y^2 <= n, excluding (0,0), divided by
 *          4.
 * https://oeis.org/A014200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A014198(n) - 1) / 4
 * Range:     u8 n <= 323, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A014198(n) by enumerating all integer x from -⌊√n⌋ to ⌊√n⌋
 * and counting admissible y for each x.  Then exclude the origin and
 * divide by 4 to obtain a(n). */

static idx_t isqrt_u64(idx_t n) {
    /* integer square root: floor(sqrt(n)) */
    idx_t lo = 0, hi = (idx_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)   /* mid*mid > n */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    idx_t x_max = isqrt_u64(n);
    seq_t total = 0;

    for (idx_t x = 0; x <= x_max; ++x) {
        idx_t rem = n - (x * x);          /* n - x²  (no overflow: x ≤ x_max) */
        idx_t y_max = isqrt_u64(rem);
        seq_t cnt = (seq_t)2 * (seq_t)y_max + 1; /* count of y for this x */

        if (x == 0)
            total += cnt;                 /* only one sign of x (0) */
        else
            total += cnt << 1;             /* +x and –x contribute the same */
    }

    total -= 1;          /* exclude (0,0) */
    total /= 4;          /* divide by 4 as required */
    return total;
}
