/*
 * A225145  Square array read by downwards antidiagonals: T(n,k) = 1 if k mod
 *          (n+1) > 0, T(n,k) = 0 if k mod (n+1) = 0.
 * https://oeis.org/A225145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   read a(n) as the nth entry when the infinite array T(i,j) = 1 if
 *            j mod (i+1) > 0 else 0 is traversed by downwards antidiagonals.
 *            Equivalent computation uses the diagonal index t =
 *            floor((sqrt(8*n-7)-1)/2), i = n - t*(t+1)/2, j = t+2-i and
 *            returns 0 if j mod (i+1) == 0, else 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find t: largest integer with t*(t+1)/2 < n */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val < n) {
            low = mid + 1;
        } else {
            if (mid == 0) { high = 0; break; }
            high = mid - 1;
        }
    }
    idx_t t = high;
    __int128 tt = (__int128)t * (t + 1) / 2;
    idx_t i = n - (idx_t)tt;
    idx_t j = t + 2 - i;
    idx_t m = i + 1;
    idx_t mod = j % m;
    return (mod == 0) ? (seq_t)0 : (seq_t)1;
}
