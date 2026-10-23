/*
 * A193212  Triangle T(n, k): T(n, 1) = n, T(n, n) = -n if n>1, T(n, k+n) =
 *          T(n, k) if n>1.
 * https://oeis.org/A193212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 5050, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computation of OEIS A193212:
   The sequence is the concatenation of rows of the triangle T(n,k).
   T(n,1)=n; T(n,n)=-n for n>1; T(n,k+n)=T(n,k);
   If gcd(n,k)>1, T(n,k)=-gcd(n,k); otherwise T(n,k)=T(k,n).
   a(n) is the n‑th element in this concatenation (offset 1). */

static inline idx_t triangular(idx_t r)
{
    /* Use 128‑bit to avoid overflow for large r. */
    __int128 t = (__int128)r * (r + 1) / 2;
    return (idx_t)t;
}

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r‑1) < n ≤ triangular(r). */
    idx_t low = 1, high = 1;
    while (triangular(high) < n)
        high <<= 1;                /* grow high until it is large enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t k = n - triangular(r - 1);   /* column index (1‑based) */

    /* Compute T(r,k) using the rules above. */
    while (1) {
        if (k == 1)
            return (seq_t)r;                      /* T(r,1)=r */

        if (k == r)
            return r > 1 ? -(seq_t)r : (seq_t)r;   /* T(r,r) = -r (except r=1) */

        idx_t g = gcd_idx(r, k);
        if (g > 1)
            return -(seq_t)g;                     /* -gcd when >1 */

        if (k > r) {                             /* periodicity: T(r,k)=T(r,k−r) */
            k -= r;
            continue;
        }
        /* Now k < r and gcd(r,k)=1; use symmetry T(r,k)=T(k,r). */
        idx_t tmp = r;
        r = k;
        k = tmp;
    }
}
