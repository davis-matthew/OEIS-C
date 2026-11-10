/*
 * A077582  Sum of terms of n-th row of A077581.
 * https://oeis.org/A077582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the n smallest positive integers coprime to n (row
 *            n of A077581). Computed with q = floor(n/phi(n)) full blocks of
 *            length n (each block k has sum k*n*phi + n*phi/2) plus a scan
 *            for the remaining n mod phi(n) coprimes
 * Range:     u8 n <= 15, u16 n <= 203, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9120 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A077582_gcd(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static idx_t A077582_phi(idx_t n)
{
    idx_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    idx_t phi = A077582_phi(n);
    idx_t q = n / phi;
    idx_t r = n % phi;
    seq_t N = (seq_t)n;
    seq_t S1 = N * (seq_t)phi / 2;
    seq_t Q = (seq_t)q;
    seq_t total = (Q * (Q - 1) / 2) * ((seq_t)phi * N) + Q * S1;
    total += (seq_t)r * Q * N;
    idx_t cnt = 0;
    for (idx_t c = 1; c <= n && cnt < r; ++c) {
        if (A077582_gcd(c, n) == 1) {
            total += (seq_t)c;
            ++cnt;
        }
    }
    return total;
}
