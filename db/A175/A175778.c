/*
 * A175778  Length of the longest positive arithmetic progression, with
 *          nonzero common difference, that sums to n.
 * https://oeis.org/A175778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = length of the longest positive arithmetic progression
 *            (with non‑zero common difference) that sums to n. Implements by
 *            checking all possible lengths k in decreasing order and testing
 *            whether an integer solution exists
 * Range:     u8 n <= 2735, u16 n <= 9717, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* helper: greatest common divisor for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: modular inverse of a modulo m (gcd(a,m)=1, m>=1) */
static idx_t mod_inv(idx_t a, idx_t m) {
    /* extended Euclid */
    idx_t b = m, u = 1, v = 0;
    while (b != 0) {
        idx_t t = a / b;
        a -= t * b;  /* a = a - t*b */
        int64_t uu = u;
        u = v;
        v = uu - t * v;  /* v = u - t*v, but we saved u in uu */
        int64_t tt = a;
        a = b;
        b = tt;  /* swap a and b */
    }
    if (a != 1) return 0;          /* inverse does not exist */
    if (u < 0) u += m;
    return (idx_t)u;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)n;   /* a(1) = 1 */

    /* maximum possible length: k such that k*(k+1)/2 <= n */
    idx_t maxk = (idx_t)((sqrt((double)8.0 * (double)n + 1.0) - 1.0) / 2.0);

    for (idx_t k = maxk; k >= 1; --k) {
        if (k == 1) return (seq_t)1;   /* single term always works */

        /* t = k*(k-1)/2   (sum of arithmetic differences) */
        idx_t t = (k * (k - 1)) / 2;

        /* minimal possible sum for length k is k + t (when a=1, d=1) */
        if (n < k + t) continue;

        idx_t g = gcd_idx(t, k);
        if (n % g != 0) continue;     /* no solution for this k */

        idx_t t_div = t / g;
        idx_t k_div = k / g;
        idx_t n_div = (n / g) % k_div;

        idx_t d0;
        if (k_div == 1) {
            /* All d are solutions modulo 1, pick smallest positive */
            d0 = 1;
        } else {
            idx_t inv = mod_inv(t_div % k_div, k_div);
            if (inv == 0) continue;     /* no inverse, shouldn't happen */
            d0 = (inv * n_div) % k_div;
            if (d0 == 0) d0 = k_div;     /* smallest positive solution */
        }

        /* maximum d that keeps a >= 1 */
        idx_t max_d = (n - k) / t;      /* integer division, t>0 here */

        /* iterate over all d ≡ d0 (mod k_div) up to max_d */
        for (idx_t d = d0; d <= max_d; d += k_div) {
            seq_t numerator = (seq_t)n - (seq_t)d * (seq_t)t;
            if (numerator <= 0) break;          /* a would be <= 0 */
            if (numerator % (seq_t)k != 0) continue;
            seq_t a_term = numerator / (seq_t)k;
            if (a_term >= (seq_t)1) {
                return (seq_t)k;                /* longest found */
            }
        }
    }
    return (seq_t)1;   /* fallback, should not happen for n>=1 */
}
