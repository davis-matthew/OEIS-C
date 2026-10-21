/*
 * A057764  Triangle T(n,k) = number of nonzero elements of multiplicative
 *          order k in Galois field GF(2^n) (n >= 1, 1 <= k <= 2^n-1).
 * https://oeis.org/A057764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   a(n) = φ(k) if k divides 2^r−1 , otherwise 0, where r is the row
 *            and k the position in that row of the flattened triangle T(n,k).
 *            T(r,k) = Eulerφ(k) if k | (2^r−1)
 * Range:     u8 n <= 1012, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euler totient for a 64‑bit integer. */
static seq_t phi_uint64(uint64_t n)
{
    seq_t res = n;
    if (n == 0) return 0;          /* not used, but safe */
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            res -= res / p;
        }
    }
    if (n > 1) res -= res / n;
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard against 0 */

    /* find row r such that cumulative count prev < n ≤ cumulative count up to r */
    idx_t r = 1;
    idx_t cum = 0;
    while (1) {
        idx_t len = ((idx_t)1 << r) - 1;   /* 2^r – 1 , r is small (≤63) */
        if (cum + len >= n) break;
        cum += len;
        ++r;
    }

    idx_t k = n - cum;                     /* column (1‑based) */

    if (r > 63) return 0;                  /* 2^r would overflow 64 bits */
    uint64_t m = ((uint64_t)1 << r) - 1;   /* 2^r – 1  */
    if (m % k != 0) return 0;

    return phi_uint64((uint64_t)k);
}
