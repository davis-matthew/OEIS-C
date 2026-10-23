/*
 * A135596  Number of positive solutions of the Diophantine x*p+y*q=p^3+q^3,
 *          where p=n-th prime, q=(n+1)-th prime.
 * https://oeis.org/A135596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive solutions of x*p + y*q = p³+q³, where
 *            p is the n-th prime and q is the (n+1)-th prime
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes, ceil_div_int128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../utils/build_primes.h"
#include "../utils/ceil_div_int128.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

/* Ensure the global prime table contains at least n+1 primes. */
static void ensure_primes(idx_t n)
{
    idx_t need = n + 1;
    if (primes_cnt >= (size_t)need)
        return;

    /* Estimate an upper bound for the (n+1)-th prime using p_n < n(ln n + ln ln n) for n>=6. */
    idx_t limit;
    if (need < 6)
        limit = 15;
    else {
        double dn = (double)need;
        double l = dn * (log(dn) + log(log(dn)));
        limit = (idx_t)(l) + 3;
    }

    free(primes);
    size_t cnt;
    build_primes(limit, &primes, &cnt);
    primes_cnt = cnt;
}

/* Extended Euclidean algorithm for 64‑bit integers. */
static int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* Modular inverse of a modulo m (m is prime, so inverse exists). */
static uint64_t modinv(uint64_t a, uint64_t m)
{
    int64_t x, y;
    int64_t g = egcd((int64_t)a, (int64_t)m, &x, &y);
    (void)g; /* g should be 1 for distinct primes */
    int64_t inv = x % (int64_t)m;
    if (inv < 0)
        inv += (int64_t)m;
    return (uint64_t)inv;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined for n = 0, return 0 */

    ensure_primes(n);

    uint64_t p = primes[n - 1];
    uint64_t q = primes[n];

    /* Compute S = p³ + q³ using 128‑bit arithmetic to avoid overflow. */
    __int128 S = (__int128)p * p * p + (__int128)q * q * q;

    /* Solve p * x ≡ S (mod q).  Since S ≡ p³ (mod q), x₀ = (p³ mod q) * inv(p, q) (mod q). */
    uint64_t inv = modinv(p % q, q);
    __int128 p3modq = (__int128)p * p * p % q;
    __int128 x0 = (p3modq * inv) % q;    /* 0 ≤ x0 < q */

    /* Corresponding y₀. */
    __int128 y0 = (S - (__int128)p * x0) / q;

    /* k range: x = x0 + k*q ≥ 1  →  k ≥ ceil((1‑x0)/q) */
    __int128 k_min = ceil_div_int128((__int128)1 - x0, q);
    /* y = y0 – k*p ≥ 1  →  k ≤ (y0‑1)/p */
    __int128 k_max = (y0 - 1) / (__int128)p;

    __int128 diff = k_max - k_min + 1;
    seq_t result = diff > 0 ? (seq_t)diff : 0;
    return result;
}
