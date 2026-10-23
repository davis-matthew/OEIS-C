/*
 * A128434  Triangle, read by rows, T(n,k) = denominator of the maximum of the
 *          k-th Bernstein polynomial of degree n; numerator is A128433.
 * https://oeis.org/A128434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = denominator T(r,k) of the k‑th Bernstein polynomial of
 *            degree r, where r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r*(r+1)/2 (row‑major order)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 66, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   pow_uint, seq_mul, seq_one, vp_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"
#include "../utils/vp_value.h"

/* Exponent of prime p in n!  (Legendre's formula). */
static idx_t vp_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    for (idx_t div = p; div <= n; div *= p) {
        e += n / div;
    }
    return e;
}

/* Test primality for small p (p <= 200000). */
static int is_prime(idx_t n)
{
    if (n < 2)
        return 0;
    if (n % 2 == 0)
        return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return 0;
    return 1;
}

/* Main function required by the framework. */
seq_t a(idx_t N)
{
    /* Find the row r such that r*(r+1)/2 <= N < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= N)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = N - start;

    /* Degenerate cases. */
    if (r <= 1 || k == 0 || k == r)
        return seq_one();

    seq_t result = seq_one();

    /* For each prime <= r we evaluate its contribution to the denominator. */
    for (idx_t p = 2; p <= r; ++p) {
        if (!is_prime(p))
            continue;

        /* Exponent of p in n = r. */
        idx_t vp_n = vp_value(r, p);
        if (vp_n == 0)
            continue;            /* p does not divide r → no effect on denominator. */

        /* Denominator exponent from n^n. */
        idx_t e_den = r * vp_n;

        /* Exponent of p in binomial(r,k). */
        idx_t e_binom = vp_factorial(r, p) - vp_factorial(k, p) - vp_factorial(r - k, p);

        /* Exponent of p in k^k and (r-k)^(r-k). */
        idx_t vp_k_val   = (k == 0) ? 0 : vp_value(k, p);
        idx_t vp_nk_val  = ((r - k) == 0) ? 0 : vp_value(r - k, p);

        /* Total exponent of p in the numerator. */
        idx_t e_num = e_binom + k * vp_k_val + (r - k) * vp_nk_val;

        /* Cancellation: remove the common part. */
        idx_t g = (e_num < e_den) ? e_num : e_den;
        idx_t exp = e_den - g;        /* exponent to keep in the denominator. */

        if (exp > 0) {
            seq_t mult = pow_uint((uint64_t)p, exp);
            result = seq_mul(result, mult);
        }
    }

    return result;
}
