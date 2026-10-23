/*
 * A036283  Write cosec x = 1/x + Sum e_n x^(2n-1)/(2n-1)!; sequence gives
 *          denominators of e_n.
 * https://oeis.org/A036283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of ((2^(2n-1)-1) * B_{2n} / n)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 35, u64 n <= 209,
 *            u128 n <= 1259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑indexed.  Uses the von Staudt–Clausen theorem for Bernoulli numbers. */
#include <stdint.h>
#include <stdbool.h>

static bool primes_init = false;
static bool is_prime[20002];

static void init_primes(void) {
    if (primes_init) return;
    const int MAX = 20001;
    for (int i = 0; i <= MAX; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX; j += i)
                is_prime[j] = false;
        }
    }
    primes_init = true;
}

/* fast modular exponentiation using 128‑bit multiplication */
static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}
static uint64_t powmod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    uint64_t x = base % mod;
    while (exp) {
        if (exp & 1) res = mulmod(res, x, mod);
        x = mulmod(x, x, mod);
        exp >>= 1;
    }
    return res;
}

/* exponent of p in 2^exp - 1 */
static uint64_t vkp(uint64_t p, uint64_t exp) {
    if (powmod(2, exp, p) != 1) return 0;
    uint64_t mod = p;
    uint64_t e = 1;
    while (true) {
        if (mod > UINT64_MAX / p) break;
        uint64_t next = mod * p;
        if (powmod(2, exp, next) == 1) {
            ++e;
            mod = next;
        } else break;
    }
    return e;
}

seq_t a(idx_t n) {
    init_primes();
    uint64_t un = (uint64_t)n;
    if (un == 0) return 0;  /* undefined for offset>0, but guard */

    uint64_t m = 2 * un;          /* 2n   */
    seq_t res = 1;

    for (uint64_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            /* divisor i */
            uint64_t d = i;
            uint64_t p = d + 1;
            if (p <= 20001 && is_prime[p]) {
                /* exponent of p in n */
                uint64_t t = un;
                uint64_t vp_n = 0;
                while (t % p == 0) { t /= p; ++vp_n; }

                /* exponent of p in k = 2^(2n-1)-1 */
                uint64_t vp_k = 0;
                if (p != 2) vp_k = vkp(p, 2 * un - 1);
                uint64_t exp = 1 + vp_n - vp_k;
                if (exp > 0) {
                    for (uint64_t j = 0; j < exp; ++j)
                        res *= (seq_t)p;
                }
            }

            /* divisor m/i  (if distinct) */
            uint64_t d2 = m / i;
            if (d2 != i) {
                uint64_t p2 = d2 + 1;
                if (p2 <= 20001 && is_prime[p2]) {
                    uint64_t t = un;
                    uint64_t vp_n = 0;
                    while (t % p2 == 0) { t /= p2; ++vp_n; }

                    uint64_t vp_k = 0;
                    if (p2 != 2) vp_k = vkp(p2, 2 * un - 1);
                    uint64_t exp = 1 + vp_n - vp_k;
                    if (exp > 0) {
                        for (uint64_t j = 0; j < exp; ++j)
                            res *= (seq_t)p2;
                    }
                }
            }
        }
    }
    return res;
}
