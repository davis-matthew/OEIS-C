/*
 * A192037  Generalized Euler phi function (for p=5).
 * https://oeis.org/A192037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^{n-m} * Π_{d|m} (5^{ord_d(5)}−1)^{φ(d)/ord_d(5)}, where
 *            m = n / 5^{v₅(n)} (largest power of 5 dividing n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* ---------- small helper functions ---------------------------------- */

/* fast modular exponentiation 5^exp mod mod */
static uint64_t powmod_uint64(uint64_t a, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t r = 1 % mod;
    a %= mod;
    while (exp) {
        if (exp & 1) r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        exp >>= 1;
    }
    return r;
}

/* Euler's totient for a 64‑bit integer, simple trial division. */
static uint64_t phi_u64(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t res = n;
    uint64_t t   = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            res -= res / p;
        }
    }
    if (t > 1) res -= res / t;
    return res;
}

/* multiplicative order of 5 modulo n; n>1 and gcd(n,5)=1. */
static uint64_t order_of_5_mod(uint64_t n, uint64_t phi)
{
    uint64_t order = phi;
    uint64_t tmp   = phi;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            while (order % p == 0 && powmod_uint64(5ULL, order / p, n) == 1)
                order /= p;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        while (order % tmp == 0 && powmod_uint64(5ULL, order / tmp, n) == 1)
            order /= tmp;
    }
    return order;
}

/* process a single divisor d of m, multiplying its contribution into *res */
static void apply_divisor(uint64_t d, seq_t *res)
{
    uint64_t phi = phi_u64(d);
    uint64_t ord;
    if (d == 1) {          /* ord(5,1)=1 by convention */
        ord = 1;
    } else {
        ord = order_of_5_mod(d, phi);
    }
    uint64_t power = phi / ord;

    seq_t pow_ord = seq_pow((seq_t)5ULL, (idx_t)ord);
    seq_t base_minus = pow_ord - (seq_t)1ULL;
    seq_t term = seq_pow(base_minus, (idx_t)power);
    *res = seq_mul(*res, term);
}

/* ---------- main sequence function --------------------------------- */

seq_t a(idx_t n)
{
    if (n == 1) return seq_one();          /* special case from OEIS */

    uint64_t nn = (uint64_t)n;

    /* separate the highest power of 5 dividing n */
    uint64_t m = nn;
    uintptr_t power5_pow = 1;              /* 5^k, but only needed as exponent */
    while (m % 5ULL == 0) {
        m /= 5ULL;
        power5_pow *= 5ULL;
    }

    /* start with 5^{n-m} */
    seq_t result = seq_pow((seq_t)5ULL, (idx_t)(nn - m));

    /* iterate over all divisors of m.
       For each divisor d we multiply its contribution.         */
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            apply_divisor(i, &result);
            if (i != m / i)
                apply_divisor(m / i, &result);
        }
    }

    return result;
}
