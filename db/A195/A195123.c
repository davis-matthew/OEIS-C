/*
 * A195123  a(n) = 2*n - floor(n*r/3), where r = (1 + sqrt(5))/2.
 * https://oeis.org/A195123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - floor(n*r/3), with r = (1 + sqrt(5)) / 2
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use 128‑bit arithmetic to avoid overflow for the internal checks.
       All values are non‑negative, but signed __int128 is used to allow
       a possible negative intermediate in `diff`. */
    __int128 nn = (__int128)n;
    __int128 a_val = (__int128)5 * nn * nn;   /* 5 * n^2  */

    idx_t low  = 0;
    idx_t high = n;       /* floor(n*phi/3) ≤ n because phi/3 < 1 */
    idx_t mid  = 0;
    idx_t m    = 0;       /* result of floor(n*phi/3) */

    while (low <= high) {
        mid = low + (high - low) / 2;
        __int128 diff = (__int128)6 * mid - nn;         /* 6*m - n */
        __int128 diff_sq = diff * diff;

        if (a_val >= diff_sq) {           /* satisfies lower bound  */
            m = mid;                      /* candidate value       */
            low = mid + 1;                /* try larger m          */
        } else {
            if (mid == 0)                 /* prevent underflow */
                break;
            high = mid - 1;
        }
    }

    /* a(n) = 2*n - floor(n*phi/3) */
    __int128 result = (__int128)2 * nn - (__int128)m;
    return (seq_t)result;
}
