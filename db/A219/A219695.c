/*
 * A219695  For odd numbers 2n - 1, half the difference between the largest
 *          divisor not exceeding the square root, and the least divisor not
 *          less than the square root.
 * https://oeis.org/A219695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A033677(2n‑1) - A033676(2n‑1)) / 2 computed as half the
 *            difference of the least divisor ≥√(2n‑1) and the greatest
 *            divisor ≤√(2n‑1) of the odd number 2n‑1
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t floor_isqrt(idx_t n)
{
    idx_t lo = 0, hi = n, res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* use division to avoid overflow */
        if (mid == 0) { res = 0; lo = 1; continue; }
        if (mid <= n / mid) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offsets < 1 */
    idx_t m = (n << 1) - 1;        /* compute 2n‑1 safely in idx_t */
    idx_t s = floor_isqrt(m);      /* floor(sqrt(2n‑1)) */

    idx_t d = s;
    while (d > 0) {
        if (m % d == 0) break;    /* largest divisor ≤ s */
        --d;
    }
    idx_t r = m / d;              /* corresponding divisor ≥ s */

    seq_t diff = (seq_t)(r - d);   /* r – d is always even */
    return diff / 2;               /* half the difference */
}
