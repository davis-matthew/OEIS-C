/*
 * A204984  a(n) = (1/n)*A204983(n).
 * https://oeis.org/A204984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^ord_{m}(2)-1)/m, where m is the odd part of n (n = 2^e
 *            * m) and ord_{m}(2) is the multiplicative order of 2 modulo m
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) → seq_t */
#include "../utils/gcd_u64.h"

/* gcd for uint64_t */

/* fast modular exponentiation for small modulus (≤ 10000) */
static uint64_t mod_pow_uint(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* multiplicative order of 2 modulo p^e (p odd prime, e≥1) */
static uint64_t order_2_mod_pe(uint64_t p, int e)
{
    /* modulus = p^e */
    uint64_t mod = 1;
    for (int i = 0; i < e; ++i) mod *= p;

    /* φ(p^e) = p^(e-1) * (p-1) */
    uint64_t phi = 1;
    for (int i = 0; i < e - 1; ++i) phi *= p;
    phi *= (p - 1);

    uint64_t order = phi;
    uint64_t tmp = phi;

    /* factor phi and try to reduce the order */
    for (uint64_t q = 2; q * q <= tmp; ++q) {
        if (tmp % q == 0) {
            while (tmp % q == 0) tmp /= q;
            while (order % q == 0) {
                uint64_t candidate = order / q;
                if (mod_pow_uint(2, candidate, mod) == 1ULL)
                    order = candidate;
                else
                    break;
            }
        }
    }
    if (tmp > 1) {
        uint64_t q = tmp;
        while (order % q == 0) {
            uint64_t candidate = order / q;
            if (mod_pow_uint(2, candidate, mod) == 1ULL)
                order = candidate;
            else
                break;
        }
    }
    return order;
}

/* multiplicative order of 2 modulo an odd integer m ≤ 10000 */
static uint64_t order_2_mod_m(uint64_t m)
{
    uint64_t lcm_order = 1;
    uint64_t tmp = m;

    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            uint64_t ord = order_2_mod_pe(p, e);
            lcm_order = (lcm_order / gcd_u64(lcm_order, ord)) * ord;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        uint64_t ord = order_2_mod_pe(tmp, 1);
        lcm_order = (lcm_order / gcd_u64(lcm_order, ord)) * ord;
    }
    return lcm_order;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset starts at 1 */

    /* odd part m of n */
    idx_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;

    if (m == 1) return (seq_t)1;               /* a(1) = 1 */

    uint64_t ord = order_2_mod_m((uint64_t)m); /* ord_2 modulo m */

    seq_t pow2 = pow_uint(2, ord);
    seq_t num   = pow2 - (seq_t)1;             /* 2^ord - 1 */
    seq_t ans   = num / (seq_t)m;              /* divide by odd part m */

    return ans;
}
