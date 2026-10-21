/*
 * A094728  Triangle read by rows: T(n,k) = n^2 - k^2, 0 <= k < n.
 * https://oeis.org/A094728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(m,k) = m*m - k*k * where m is the unique integer such
 *            that (m-1)*m/2 < n ≤ m*(m+1)/2, * and k = n - (m-1)*m/2 - 1. *
 *            This implements the triangle T(n,k) = n² – k² read by rows
 * Range:     u8 n <= 44, u16 n <= 4908, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle empty or invalid input – the specification expects n ≥ 1. */
    if (n == 0)
        return 0;

    /* Binary search for the row number m */
    idx_t low = 1, high = n;      /* high is always large enough: n(n+1)/2 ≥ n */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        seq_t midS = (seq_t)mid;
        /* mid*(mid+1)/2 could overflow idx_t, so use seq_t */
        seq_t s = midS * (midS + 1) / 2;
        if (s >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Number of terms before row m */
    seq_t offset = (seq_t)(m - 1) * m / 2;

    /* Column index within the row (0‑based) */
    idx_t k = (idx_t)((seq_t)n - offset - 1);

    /* Value of the triangle entry */
    seq_t val = (seq_t)m * m - (seq_t)k * k;
    return val;
}
