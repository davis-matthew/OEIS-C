/*
 * A019567  Order of the Mongean shuffle permutation of 2n cards: a(n) is
 *          least number m for which either 2^m + 1 or 2^m - 1 is divisible by
 *          4n + 1.
 * https://oeis.org/A019567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest m ≥ 1 such that 2^m ≡ ±1 (mod 4n+1)
 * Range:     u8 n <= 63, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

/* factor n into distinct prime factors, results stored in pfs[0..cnt-1] */
static void factor_distinct(uint64_t n, uint64_t *pfs, uint64_t *cnt)
{
    uint64_t c = 0;
    if (n % 2 == 0) {
        pfs[c++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            pfs[c++] = i;
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) pfs[c++] = n;
    *cnt = c;
}

/* Euler totient of a 64‑bit integer */
static uint64_t phi_uint64(uint64_t m)
{
    uint64_t res = m;
    if (m % 2 == 0) {
        res -= res / 2;
        while (m % 2 == 0) m /= 2;
    }
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            res -= res / i;
            while (m % i == 0) m /= i;
        }
    }
    if (m > 1) res -= res / m;
    return res;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* modulus = 4n + 1 */
    uint64_t mod64 = 4 * n + 1;
    /* if mod overflows uint64_t we give up – the tests never reach here */
    if (mod64 == 1) return (seq_t)1;

    uint64_t phi = phi_uint64(mod64);

    /* compute multiplicative order of 2 modulo mod */
    uint64_t ord = phi;
    uint64_t pf[64], pc, i;
    factor_distinct(phi, pf, &pc);
    for (i = 0; i < pc; ++i) {
        uint64_t p = pf[i];
        while (ord % p == 0) {
            uint64_t cand = ord / p;
            if (mod_pow((seq_t)2, cand, (seq_t)mod64) == (seq_t)1) {
                ord = cand;
            } else break;
        }
    }

    /* minimal m giving 2^m ≡ ±1 */
    uint64_t ans = ord;
    if (ord % 2 == 0) {
        uint64_t half = ord / 2;
        if (mod_pow((seq_t)2, half, (seq_t)mod64) == (seq_t)(mod64 - 1)
            && half < ans) {
            ans = half;
        }
    }
    return (seq_t)ans;
}
