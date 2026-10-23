/*
 * A179519  'AP(n,k)' triangle read by rows. AP(n,k) is the number of
 *          aperiodic k-palindromes of n.
 * https://oeis.org/A179519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle AP(n,k) (number of
 *            aperiodic k‑palindromes of n) row‑wise. For the triangle entry
 *            (r,k) we use AP(r,k) = Σ_{d|gcd(r,k)} μ(d) · Pal(r/d , k/d)
 *            where μ is the Möbius function and Pal(N,K) = number of
 *            K‑palindromic compositions of N, computed by the closed fo...
 * Range:     u8 n <= 261, u16 n <= 757, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   mu_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mu_small.h"

/* helper: Euclidean gcd for idx_t (uint64_t) */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: binomial coefficient C(n,k) for idx_t arguments */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / i;
    }
    return res;
}

/* helper: number of K‑palindromic compositions of N */
static seq_t pal_count(idx_t n, idx_t k)
{
    if (k == 0) return (n == 0) ? 1 : 0;
    if (k > n) return 0;

    if ((k & 1) == 0) {                     /* k even */
        if (n & 1) return 0;                /* n odd -> impossible */
        idx_t s   = k >> 1;
        idx_t half = n >> 1;                 /* n/2 */
        if (half < s) return 0;
        return binom_small(half - 1, s - 1);
    } else {                                 /* k odd */
        idx_t s   = k >> 1;                     /* floor((k-1)/2) */
        idx_t m   = (n - 1) >> 1;               /* floor((n-1)/2) */
        if (m < s) return 0;
        return binom_small(m, s);
    }
}

/* helper: find row r such that r(r+1)/2 >= n */
static idx_t row_from_index(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    idx_t r = row_from_index(n);
    __int128 prev = (__int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev;                 /* 1 <= k <= r */

    idx_t g = gcd_idx(r, k);
    seq_t pos = 0, neg = 0;

    for (idx_t d = 1; d <= g; ++d) {
        if (g % d != 0) continue;
        int mu = mu_small(d);
        if (mu == 0) continue;

        idx_t r_d = r / d;
        idx_t k_d = k / d;
        seq_t p   = pal_count(r_d, k_d);

        if (mu > 0) pos += p;
        else        neg += p;
    }

    /* final result is non‑negative */
    return pos - neg;
}
