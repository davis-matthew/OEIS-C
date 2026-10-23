/*
 * A144475  A triangle sequence of determinants: a(n)=If[Mod[n, 3] == 0, 1,
 *          If[Mod[n, 3] == 1, -1, If[Mod[n, 3] == 2, 0]]]; b(n,m)=If[m < n &&
 *          Mod[m, 3] == 0, 0, If[m < n && Mod[m, 3] == 1, 0, If[m < n &&
 *          Mod[m, 3] == 2 && Mod[n, 2] == 0, 1, If[m < n && Mod[m, 3] == 2 &&
 *          Mod[n, 2] == 1, -1, If[m == n, -1, 0]...
 * https://oeis.org/A144475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, uned
 * Formula:   The sequence enumerates the determinants t(n,m) = Det[ {a(m),
 *            b(n,m)} , {a(n), b(n,n)} ] for 0 ≤ m < n, with a(k) = 1 (k mod 3
 *            = 0) = -1 (k mod 3 = 1) = 0 (k mod 3 = 2) b(n,m) = 0 (m < n and
 *            m mod 3 = 0 or 1) = 1 / -1 (m < n, m mod 3 = 2, n even / odd) =
 *            -1 (m = n) The terms are listed row‑by‑row...
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: triangular number in 128‑bit to avoid overflow. */
static inline __int128 tri128(idx_t x)
{
    return (__int128)x * (x + 1) / 2;
}

seq_t a(idx_t k)
{
    /* Find the smallest n such that n*(n+1)/2 >= k. */
    idx_t low = 1, high = 1;
    while (tri128(high) < ( __int128)k)
        high <<= 1;               /* double until high is large enough   */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (tri128(mid) >= ( __int128)k)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t n = low;                          /* current row (1‑based)              */
    idx_t prevTri = (n > 1) ? (n - 1) * n / 2 : 0;   /* T(n-1)                     */
    idx_t m = k - prevTri - 1;              /* column index 0‑based              */

    /* a(n) for the current row */
    seq_t row_val;
    switch (n % 3) {
        case 0:  row_val =  1; break;
        case 1:  row_val = -1; break;
        default: row_val =  0; break;
    }

    /* Compute determinant value t(n,m) */
    seq_t res;
    if (m % 3 == 0) {
        /* a(m)=1, b=0, so t = -1 */
        res = -1;
    } else if (m % 3 == 1) {
        /* a(m)=-1, b=0, so t =  1 */
        res =  1;
    } else { /* m % 3 == 2 */
        if ((n & 1) == 0) {   /* n even → b = 1  → t = -row_val */
            res = -row_val;
        } else {              /* n odd  → b = -1 → t =  row_val */
            res =  row_val;
        }
    }

    return res;
}
