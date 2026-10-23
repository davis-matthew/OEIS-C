/*
 * A013617  Triangle of coefficients in expansion of (1+10*x)^n.
 * https://oeis.org/A013617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = binom(r,k) * 10^k where r = largest integer with
 *            r*(r+1)/2 ≤ n and k = n - r*(r+1)/2, 0 ≤ k ≤ r. The value equals
 *            the (n‑th) element of the flattened triangle of coefficients of
 *            (1+10*x)^r. The recursion T(n,k)=10*T(n−1,k−1)+T(n−1,k) is
 *            equivalent
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)          return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k)      k = n - k;          /* symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res * (n - k + i) / i  */
        res = (res * (n - k + i)) / i;
    }
    return res;
}

static seq_t pow10(idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        res *= 10;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (1) {
        unsigned __int128 next = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (next > n) break;
        ++r;
    }

    idx_t start = r * (r + 1) / 2;
    idx_t k     = n - start;          /* 0 ≤ k ≤ r */

    seq_t bin  = binomial(r, k);
    seq_t p10  = pow10(k);
    return bin * p10;
}
