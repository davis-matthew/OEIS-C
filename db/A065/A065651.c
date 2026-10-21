/*
 * A065651  a(n) = Sum_{k=1..n} (-1)^tau(k) = n - 2*floor(sqrt(n)).
 * https://oeis.org/A065651
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 2 * ⌊√n⌋ , offset 2
 * Range:     u8 n <= 287, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n ≥ 2.  Uses only integer arithmetic. */
seq_t a(idx_t n)
{
    /* For indices below the offset the result is irrelevant. */
    if (n < 2)
        return 0;

    /* Binary search for floor(sqrt(n)).  The square of any candidate
       fits in idx_t (≤ 2^64-1) because the maximal square root is 2^32-1. */
    idx_t lo = 0, hi = n, s = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t sq  = mid * mid;
        if (sq == n) {
            s = mid;
            break;
        } else if (sq < n) {
            s = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* cannot go lower */
            hi = mid - 1;
        }
    }

    /* n - 2 * s is non‑negative for n ≥ 2, so it fits in seq_t. */
    return (seq_t)n - (seq_t)2 * (seq_t)s;
}
