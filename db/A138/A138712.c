/*
 * A138712  n-th run has length n-th positive triangular number, with digits 0
 *          and 1 only, starting with 0.
 * https://oeis.org/A138712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if the run index r with S(r‑1) < n ≤ S(r) is odd, a(n)
 *            = 1 if r is even, where S(r) = r(r+1)(r+2)/6. The run lengths
 *            are the triangular numbers T_k = k(k+1)/2
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A138712 is defined for n ≥ 1; for n < 1 return 0 as a safe default. */
    if (n < 1) return 0;

    idx_t low = 1, high = 1;
    /* Find an upper bound high such that S(high) ≥ n. */
    while (1) {
        unsigned __int128 s = (unsigned __int128)high * (high + 1) * (high + 2) / 6;
        if (s >= n) break;
        high <<= 1;               /* double until we surpass the target */
    }
    /* Binary search for the smallest r with S(r) ≥ n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) * (mid + 2) / 6;
        if (s >= n)
            high = mid;
        else
            low = mid + 1;
    }
    /* low (or high) is the run index r.  Odd runs are 0, even runs are 1. */
    return (low % 2 == 0) ? 1 : 0;
}
