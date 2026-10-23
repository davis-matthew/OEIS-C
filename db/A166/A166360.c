/*
 * A166360  Triangle of Narayana numbers mod 2, T(n,k) = A001263(n,k) mod 2,
 *          read by rows.
 * https://oeis.org/A166360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (N(r,k) mod 2), where r = smallest integer with r(r+1)/2
 *            ≥ n, k = n - r(r-1)/2 (1 ≤ k ≤ r), N(r,k) = (1/r) * C(r,k) *
 *            C(r,k-1) (Narayana number)
 * Range:     u8 n <= 7260, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 >= n */
    idx_t low = 0;
    idx_t high = 1;
    while (( (__int128)high * (high + 1) / 2 ) < n) {
        high <<= 1;
    }
    while ((low + 1) < high) {
        idx_t mid = (low + high) >> 1;
        if (( (__int128)mid * (mid + 1) / 2 ) >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;

    /* Compute k = n - (r-1)*r/2 */
    idx_t prevTri = (idx_t)(( (__int128)(r - 1) * r ) / 2);
    idx_t k = n - prevTri;          /* 1-indexed column */

    /* Compute 2-adic valuation of Narayana number:
       e2(N) = popcount(k) + popcount(r-k) + popcount(k-1) + popcount(r-k+1)
               - 2*popcount(r) - v2(r)  */
    unsigned long long rr = (unsigned long long)r;
    unsigned long long kk = (unsigned long long)k;
    unsigned long long rmkk = rr - kk;          /* r - k   */
    unsigned long long rmkk1 = rr - kk + 1;     /* r - k + 1 */
    unsigned long long km1 = kk - 1;            /* k - 1   */

    unsigned long long pc_k   = __builtin_popcountll(kk);
    unsigned long long pc_rmk = __builtin_popcountll(rmkk);
    unsigned long long pc_km1 = __builtin_popcountll(km1);
    unsigned long long pc_rmk1= __builtin_popcountll(rmkk1);
    unsigned long long pc_r   = __builtin_popcountll(rr);
    unsigned long long v2_r   = __builtin_ctzll(rr);   /* exponent of 2 in r */

    unsigned long long e2 = pc_k + pc_rmk + pc_km1 + pc_rmk1
                            - 2 * pc_r - v2_r;

    return (seq_t)(e2 == 0 ? 1 : 0);
}
