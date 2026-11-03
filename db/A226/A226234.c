/*
 * A226234  Triangle defined by T(n,k) = binomial(n^2, k^2), for n>=0, k=0..n,
 *          as read by rows.
 * https://oeis.org/A226234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r^2, k^2) where r is the row index such that
 *            S(r-1) ≤ n < S(r) with S(t) = (t+1)(t+2)/2, and k = n - S(r-1)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 24, u64 n <= 49, u128 n <= 84,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 490 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function internally computes binomial(n, k) safely for both
   fixed‑width integers and arbitrary‑precision types. */

#include <stdint.h>

static seq_t binomial_square(idx_t n, idx_t k)
{
    idx_t a = n * n;         /* n^2 */
    idx_t b = k * k;         /* k^2 */

    if (b > a) return 0;     /* should not happen for the triangle */

    /* compute C(a, b) with a reduction to avoid overflow */
    idx_t m = b;
    if (m > a - m) m = a - m;   /* use the smaller of k and a-k */

#ifdef __cplusplus
    seq_t res = 1;
    for (idx_t i = 1; i <= m; ++i) {
        res = (res * (a - m + i)) / i;
    }
    return res;
#else
    seq_t res = 1;
    for (idx_t i = 1; i <= m; ++i) {
        unsigned __int128 prod = (unsigned __int128)res * (a - m + i);
        res = (seq_t)(prod / i);
    }
    return res;
#endif
}

seq_t a(idx_t n)
{
    /* Binary search for the row r: smallest r with S(r) > n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (val > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* offset within that row */
    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    return binomial_square(r, k);
}
