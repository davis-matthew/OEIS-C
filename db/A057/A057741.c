/*
 * A057741  Table T(n,k) giving number of elements of order k in dihedral
 *          group D_{2n} of order 2n, n >= 1, 1<=k<=g(n), where g(n) = 2 if
 *          n=1 else g(n) = n.
 * https://oeis.org/A057741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   phi_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_value.h"
/*  A057741 – number of elements of order k in D_{2n}, row‑wise indexed.
 *  For the row r (the n in D_{2r}) and column k (1 ≤ k ≤ g(r))
 *  with g(1)=2 and g(r)=r for r>1,
 *
 *  T(r,k) =  { (r odd  ? r       : r+1)        if k==2,
 *             {  φ(k)                       if k!=2 and k|r,
 *             {  0                          otherwise }      (φ = Euler totient)
 *
 *  The linear index n (starting at 1) is formed by listing the rows
 *  one after another:
 *     n = 1 → T(1,1),  n = 2 → T(1,2),
 *     n = 3 → T(2,1),  n = 4 → T(2,2),
 *     n = 5 → T(3,1),  n = 6 → T(3,2), …
 */

static idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t lo = 0, hi = (idx_t)-1;      /* hi = maximum 64‑bit value */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * (unsigned __int128)mid;
        if (mid2 == v) return mid;
        if (mid2 < v) lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;                         /* floor(sqrt(v)) */
}

static idx_t find_row(idx_t n)
{
    /* Finds r such that 1 + r(r+1)/2 ≥ n  (row index, r ≥ 1) */
    if (n == 1) return 1;

    unsigned __int128 w = 8 * (n - 1) + 1;      /* 1 + 8(n-1) */
    idx_t s = isqrt_u128(w);
    idx_t r = (s - 1) >> 1;                    /* floor((sqrt(w)-1)/2) */
    if (r < 1) r = 1;

    /* Correct possible off‑by‑one errors of the integer rounding */
    while (1 + (unsigned __int128)r * (r + 1) / 2 < n) ++r;
    while (r > 1 && 1 + (unsigned __int128)(r - 1) * r / 2 >= n) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* safety for undefined 0 */

    idx_t r = find_row(n);
    idx_t prev = (r == 1) ? 0
                          : 1 + (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;                          /* column index (1‑based) */

    if (k == 2) {                                 /* elements of order 2 */
        return (r & 1) ? (seq_t)r : (seq_t)(r + 1);    /* r odd → r; even → r+1 */
    }

    if (r % k == 0)                               /* other orders dividing r */
        return phi_value(k);

    return 0;                                      /* no elements of such order */
}
