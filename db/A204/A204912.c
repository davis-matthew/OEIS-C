/*
 * A204912  Ordered differences of double factorials.
 * https://oeis.org/A204912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m!! – k!! where m is the smallest integer with m*(m‑1)/2
 *            ≥ n and k = n – (m‑2)*(m‑1)/2. This enumerates the differences
 *            of double factorials row by row: for each m ≥ 2 the m‑1 terms
 *            m!!‑k!! (k=1…m‑1)
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t double_factorial(idx_t x)
{
    seq_t r = 1;
    for (idx_t i = x; i > 1; i -= 2)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    /* Find m such that m*(m‑1)/2 ≥ n */
    idx_t low = 2, high = 2;
    while (((__int128)high * (high - 1) / 2) < n)
        high <<= 1;                     /* exponential search */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid - 1) / 2) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Position within the row */
    idx_t tri_prev = ((__int128)(m - 1) * (m - 2) / 2);
    idx_t pos = n - tri_prev;            /* 1 ≤ pos ≤ m‑1 */
    idx_t k = pos;

    /* Compute and return the difference */
    return double_factorial(m) - double_factorial(k);
}
