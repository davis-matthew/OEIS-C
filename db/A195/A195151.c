/*
 * A195151  Square array read by antidiagonals upwards: T(n,k) =
 *          n*((k-2)*(-1)^n+k+2)/4, n >= 0, k >= 0.
 * https://oeis.org/A195151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, mult
 * Formula:   a(n) is the n‑th element of the square array * T(n,k) =
 *            n*((k-2)*(-1)^n + k + 2)/4 enumerated by antidiagonals upwards.
 *            * For the diagonal d (where d*(d+1)/2 ≤ n < (d+1)*(d+2)/2) we
 *            set * offset = n - d*(d+1)/2, * i = d - offset, j = offset. *
 *            Then T(i,j) simplifies to: * if i is odd -> i * if i...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* search for the largest d with d*(d+1)/2 ≤ n */
    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t d = lo;

    unsigned __int128 cum128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t cum = (idx_t)cum128;
    idx_t offset = n - cum;

    idx_t i = d - offset;
    idx_t j = offset;

    if (i & 1U) {
        return (seq_t)i;
    } else {
        return (seq_t)((i >> 1) * (seq_t)j);
    }
}
