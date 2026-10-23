/*
 * A144824  Triangle read by rows, A054533 * A127648 (matrix product).
 * https://oeis.org/A144824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = T(r,k) T(r,k) = k * Σ_{d | gcd(r,k)} d * μ(r/d) where r
 *            is the row number satisfying r(r+1)/2 ≥ n and k = n - (r-1)r/2
 *            (column index in that row). μ(x) is the Möbius function
 * Range:     i8 n <= 90, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_idx.h"

/* Euclidean algorithm for unordered indices */

/* Möbius function for small indices (≤ r ≤ √(2n)) */
static int mobius_idx(idx_t n) {
    if (n == 1) return 1;
    int sign = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                if (exp > 1) return 0; /* squared prime factor */
            }
            sign = -sign; /* odd number of prime factors */
        }
    }
    if (temp > 1) sign = -sign; /* remaining prime factor */
    return sign;
}

/* Main function computing a(n) */
seq_t a(idx_t n) {
    /* Find the row r containing entry n */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }
    /* k is the position within the found row */
    idx_t prev_cum = cum - r;   /* cumulative entries before row r */
    idx_t k = n - prev_cum;     /* 1‑based column index */

    /* Compute T(r,k) with the given sum */
    idx_t g = gcd_idx(r, k);
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            int mu1 = mobius_idx(r / d);
            sum += (seq_t)d * (seq_t)mu1;
            if (d != g / d) {
                idx_t d2 = g / d;
                int mu2 = mobius_idx(r / d2);
                sum += (seq_t)d2 * (seq_t)mu2;
            }
        }
    }
    return (seq_t)k * sum;
}
