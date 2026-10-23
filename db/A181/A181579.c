/*
 * A181579  Smallest number m such that m! ends in exactly n trailing 0's (or
 *          0 if no such m exists).
 * https://oeis.org/A181579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest m such that m! ends in exactly n trailing zeros.
   Returns 0 if no such m exists. */
static inline idx_t trailing_zeros(idx_t m)
{
    idx_t z = 0;
    while (m) {
        m /= 5;
        z += m;
    }
    return z;
}

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0) return (seq_t)0;

    /* Upper bound: for any m, f(m) >= floor(m/5).
       Hence the minimal m with f(m) >= n is <= 5*n. */
    idx_t low  = 0;
    idx_t high = 5 * n;          /* safe for n up to ~10^19 on 64‑bit */
    if (high == 0) high = 5;     /* handle n=0 (already handled) */

    /* Binary search for minimal m with f(m) >= n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (trailing_zeros(mid) < n)
            low = mid + 1;
        else
            high = mid;
    }

    /* low is the minimal m with f(m) >= n.  Check if it gives exactly n. */
    return (trailing_zeros(low) == n) ? (seq_t)low : (seq_t)0;
}
