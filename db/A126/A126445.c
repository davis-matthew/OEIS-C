/*
 * A126445  Triangle, read by rows, where T(n,k) = C(C(n+2,3) - C(k+2,3), n-k)
 *          for n >= k >= 0.
 * https://oeis.org/A126445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the triangle with T(r,k) = C( C(r+2,3)
 *            – C(k+2,3), r – k ), read by rows, offset 0
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 65, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor sqrt for 64‑bit unsigned integers */
static idx_t floor_isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);          /* > sqrt(UINT64_MAX) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            if (sq == v) return mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* C(x,3) = x*(x-1)*(x-2)/6, x is an unsigned integer */
static seq_t comb3(idx_t x)
{
    return ((seq_t)x * (x - 1) * (x - 2)) / 6;
}

/* binomial coefficient C(n,k) for arbitrary‑precision n and k <= n.
   Uses only + - * / operators on seq_t.                           */
static seq_t binom_seq_t(seq_t n, idx_t k)
{
    if ((seq_t)k > n) return 0;
    if (k == 0) return 1;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - (seq_t)k + (seq_t)i;
        res = (res * num) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t D = 8 * n + 1;
    idx_t s = floor_isqrt_u64(D);
    idx_t r = (s - 1) / 2;

    /* adjust r if necessary (safety for edge cases) */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;                       /* column index */

    seq_t N = comb3(r + 2) - comb3(k + 2);
    idx_t K = r - k;
    return binom_seq_t(N, K);
}
