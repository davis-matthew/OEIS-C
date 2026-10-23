/*
 * A159819  Coefficients of L-series for elliptic curve "48a4": y^2 = x^3 +
 *          x^2 + x.
 * https://oeis.org/A159819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(2*n+1) b is multiplicative with b(1) = 1, b(2^e) = 0 (e
 *            ≥ 1), b(3^e) = 1, b(p^e) = b(p)·b(p^(e‑1)) – p·b(p^(e‑2)) for
 *            primes p ≠ 2,3
 * Range:     i8 n <= 1160, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ---------- integer square root ---------- */
static uint64_t uint64_sqrt(uint64_t n)
{
    uint64_t r = 0, c = 1ULL << 32;
    while (c) {
        uint64_t t = r + c;
        if (t <= n / t) r = t;
        c >>= 1;
    }
    return r;
}

/* ---------- modular exponentiation ---------- */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t b   = base % mod;
    while (exp) {
        if (exp & 1ULL) res = (uint64_t)((__uint128_t)res * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1ULL;
    }
    return res;
}

/* ---------- trace of Frobenius for prime p (p > 3) ---------- */
static seq_t compute_trace(uint64_t p)
{
    seq_t sum = 0;
    for (uint64_t x = 0; x < p; ++x) {
        uint64_t x2 = (x * x) % p;
        uint64_t x3 = (x2 * x) % p;
        uint64_t rhs = (x3 + x2 + x) % p;
        if (rhs == 0) continue;
        uint64_t leg = mod_pow(rhs, (p - 1) >> 1, p);
        if (leg == 1)      ++sum;
        else if (leg == p - 1) --sum;   /* Legendre symbol –1  */
    }
    return (seq_t)(-sum);                /* a_p = – Σ (f(x)/p)      */
}

/* ---------- value of b(p) for any prime p (p > 0) ---------- */
static seq_t compute_bp(uint64_t p)
{
    if (p == 2) return 0;            /* b(2^e) = 0 for e ≥ 1      */
    if (p == 3) return 1;            /* b(3^e) = 1 for all e     */
    return compute_trace(p);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    uint64_t N = 2 * (uint64_t)n + 1;   /* argument of b: odd integer */
    seq_t result = 1;
    uint64_t temp = N;

    while (temp > 1) {
        uint64_t limit = uint64_sqrt(temp);

        /* trial division by odd numbers up to sqrt(temp) */
        uint64_t p = 3;
        int found = 0;
        for (; p <= limit; p += 2) {
            if (temp % p == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {                 /* temp itself is prime */
            seq_t val = compute_bp(temp);
            result *= val;
            break;
        }

        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        seq_t val;
        if (p == 2)          val = 0;  /* never occurs for odd N, safety */
        else if (p == 3)     val = 1;
        else {
            seq_t bp = compute_bp(p);
            if (exp == 1) val = bp;
            else {
                seq_t a0 = 1;
                seq_t a1 = bp;
                for (int k = 2; k <= exp; ++k) {
                    seq_t a2 = a1 * bp - (seq_t)p * a0;
                    a0 = a1;
                    a1 = a2;
                }
                val = a1;
            }
        }
        result *= val;
    }

    return result;
}
