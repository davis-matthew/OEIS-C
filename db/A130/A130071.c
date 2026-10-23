/*
 * A130071  Triangle, A007444(k) in each column interspersed with k zeros.
 * https://oeis.org/A130071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if c does not divide r, * otherwise a(n) = Σ_{d|c} μ(d)
 *            * p(c/d), * where r = ceil((√(8n+1)-1)/2), c = n - r(r-1)/2, * μ
 *            is the Möbius function, and p(k) is the k‑th prime
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mu_of.h"
#include "../utils/get_prime.h"

/* Binary search for smallest r such that r*(r+1)/2 >= n. */
static inline idx_t triangular_root(idx_t n)
{
    idx_t low = 1, high = n, r = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n) {
            r = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* n>=1 by definition, but guard anyway */

    /* Determine the row r and column c of the flattened triangle. */
    idx_t r = triangular_root(n);
    idx_t prev = (idx_t)((__uint128_t)r * (r - 1) / 2);
    idx_t c = n - prev;          /* column index (1‑based) */

    /* If the column does not divide the row, the entry is zero. */
    if (r % c != 0) return 0;

    /* Compute A007444(c) = Σ_{d|c} μ(d) * p(c/d). */
    int64_t sum = 0;
    for (idx_t d = 1; (__uint128_t)d * d <= c; ++d) {
        if (c % d == 0) {
            int mu1 = mu_of(d);
            if (mu1 != 0) {
                seq_t prime1 = get_prime(c / d);
                sum += (int64_t)mu1 * (int64_t)prime1;
            }
            idx_t d2 = c / d;
            if (d2 != d) {
                int mu2 = mu_of(d2);
                if (mu2 != 0) {
                    seq_t prime2 = get_prime(d);
                    sum += (int64_t)mu2 * (int64_t)prime2;
                }
            }
        }
    }
    return (seq_t)sum;
}
