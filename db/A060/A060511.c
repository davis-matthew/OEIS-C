/*
 * A060511  Hexagonal excess: smallest amount by which n exceeds a hexagonal
 *          number (2k^2-k, A000384).
 * https://oeis.org/A060511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - (2*k*k - k) where k is the largest integer with 2*k*k
 *            - k <= n. This is equivalent to the excess of n over the nearest
 *            hexagonal number
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binary search for k such that H(k) <= n < H(k+1) */
    idx_t lo = 0;
    idx_t hi = n / 2 + 2;          /* safe upper bound for k */
    idx_t best = 0;

    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* compute H(mid) = 2*mid*mid - mid in 128‑bit to avoid overflow */
        unsigned __int128 h = (__int128)mid * (2 * (__int128)mid - 1);
        if (h <= (unsigned __int128)n) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid unsigned underflow */
            hi = mid - 1;
        }
    }

    /* H(best) converted to seq_t */
    seq_t hex_val = (seq_t)((__int128)best * (2 * (__int128)best - 1));
    /* n converted to seq_t */
    seq_t n_val   = (seq_t)n;
    return n_val - hex_val;
}
