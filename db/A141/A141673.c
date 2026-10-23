/*
 * A141673  Triangle T(n,m) read by rows: T(n,1) = n; T(n,m) = A008683(n)*n/m
 *          if m is a divisor of n, where m = 2..n, otherwise T(n,m) = 0.
 * https://oeis.org/A141673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   Triangle T(n,1)=n; T(n,m)=μ(n)*n/m if m|n, else 0. * The
 *            sequence a(n) is obtained by reading the triangle row‑by‑row
 * Range:     i8 n <= 5050, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Internal helper: compute the Möbius function μ(n). */
static seq_t _mobius(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    idx_t nn = n;
    int sign = 1;
    for (idx_t p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            int cnt = 0;
            while (nn % p == 0) {
                nn /= p;
                ++cnt;
                if (cnt > 1)
                    return (seq_t)0;   /* square factor -> 0 */
            }
            sign = -sign;   /* one prime factor contributes a sign change */
        }
    }
    if (nn > 1)
        sign = -sign;   /* remaining prime factor */
    return (seq_t)sign;
}

/* Main entry function. */
seq_t a(idx_t pos)
{
    if (pos == 0)
        return (seq_t)0;   /* undefined, but return 0 for safety */

    /* Find the row r such that r(r+1)/2 >= pos. */
    idx_t lo = 1, hi = 0xFFFFFFFF;       /* upper bound for r (2^32-1) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) >> 1;
        if (sum >= pos)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Column within the row. */
    unsigned __int128 prev_sum = (unsigned __int128)r * (r - 1) >> 1;
    idx_t col = pos - (idx_t)prev_sum;   /* 1‑based */

    if (col == 1)
        return (seq_t)r;          /* first entry of every row */

    if (r % col != 0)
        return (seq_t)0;          /* non‑divisor */

    seq_t mu = _mobius(r);
    return (mu * (seq_t)r) / (seq_t)col;
}
