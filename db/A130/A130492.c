/*
 * A130492  a(n) = denominator of Sum_{k=1..n} k^mu(n+1-k), where mu(m) =
 *          A008683(m).
 * https://oeis.org/A130492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of * Sum_{k = 1..n} k^mu(n+1-k), where mu is
 *            the Möbius function. * For each n, let S = { k | 1 ≤ k ≤ n and
 *            mu(n+1-k) = –1 }. * L = lcm_{k∈S} k. * C = Σ_{k∈S} L/k (integer
 *            sum). * Then a(n) = L / gcd(L, C)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static uint32_t * _spf = NULL;          /* smallest prime factor for each integer */
static idx_t     _spf_max = 0;          /* current maximum value with valid _spf entries */

/* Recompute the smallest‑prime‑factor array up to n. */
static void _ensure_spf(idx_t n)
{
    if (n <= _spf_max)
        return;
    _spf = (uint32_t *)realloc(_spf, (n + 1) * sizeof(uint32_t));
    if (!_spf)
        abort();
    for (idx_t i = 1; i <= n; ++i)
        _spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (_spf[i] == 0) {
            _spf[i] = (uint32_t)i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (_spf[j] == 0)
                        _spf[j] = (uint32_t)i;
                }
            }
        }
    }
    _spf_max = n;
}

/* Return true if mu(m) == -1 (square‑free with odd number of prime factors). */
static bool _mu_minus_one(idx_t m)
{
    if (m < 2)            /* mu(1) = 1 */
        return false;
    int prime_count = 0;
    idx_t x = m;
    while (x > 1) {
        uint32_t p = _spf[x];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
            if (exp > 1)   /* square factor -> mu = 0 */
                return false;
        }
        ++prime_count;
    }
    return (prime_count & 1) != 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    _ensure_spf(n);          /* need spf for computing mu */

    seq_t lcm = 1;           /* b = lcm of all k with mu(n+1-k) = -1 */
    /* First pass: compute lcm */
    for (idx_t m = 1; m <= n; ++m) {
        if (_mu_minus_one(m)) {
            seq_t k = (seq_t)(n - m + 1);   /* k = n+1-m */
            seq_t g = gcd(lcm, k);
            lcm = (lcm / g) * k;          /* lcm(lcm,k) */
        }
    }

    /* Second pass: compute C = Σ L/k (mod L) and reduce modulo L */
    seq_t c = 0;            /* sum modulo lcm */
    if (lcm != 1) {        /* avoid division by zero */
        for (idx_t m = 1; m <= n; ++m) {
            if (_mu_minus_one(m)) {
                seq_t k = (seq_t)(n - m + 1);
                seq_t term = lcm / k;   /* integer division, exact */
                c += term;
                if (c >= lcm)
                    c -= lcm;          /* keep c < lcm to avoid overflow */
            }
        }
    }

    seq_t g = gcd(lcm, c);
    return lcm / g;         /* denominator of the reduced sum */
}
