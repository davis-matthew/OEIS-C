/*
 * A030528  Triangle read by rows: T(n,k) = binomial(k,n-k).
 * https://oeis.org/A030528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 369, u32 n <= 1310, u64 n <= 4821,
 *            u128 n <= 7875, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows
 * T(n,k) = binomial(k,n-k),   1 <= k <= n
 * a(n) is the sequence obtained by flattening the rows.
 * The function maps the index n to its row r and column k,
 * then returns the binomial value.
 */
#include <stdbool.h>
#include "../utils/gcd.h"

/* triangular number as unsigned __int128 */
static inline unsigned __int128 tri128(idx_t n)
{
    return (unsigned __int128)n * (n + 1) / 2;
}

/* find the smallest r such that tri(r) >= n (n >= 1) */
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while (tri128(hi) < (unsigned __int128)n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (tri128(mid) >= (unsigned __int128)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* compute binomial(n,k) exactly, using only seq_t operations */
static seq_t binomial_exact(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)
        k = n - k;      /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g1 = gcd(result, den);
        result /= g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;      /* den should now be 1 */

        result = result * num;
    }
    return result;
}

/* external function as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* safety, offset starts at 1 */

    idx_t r = find_row(n);        /* row index (1‑based)   */
    idx_t prev_tri = (idx_t)tri128(r - 1);   /* entries before row r */
    idx_t k = n - prev_tri;       /* column index (1‑based) */
    idx_t choose = r - k;         /* lower argument of binomial */

    return binomial_exact(k, choose);
}
