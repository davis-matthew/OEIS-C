/*
 * A023136  Number of cycles of function f(x) = 4x mod n.
 * https://oeis.org/A023136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 685, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023136: number of cycles of f(x)=4x mod n
 *
 * Formula: a(n) = Sum_{d|m} phi(d) / ord_4(d),
 * where m is the odd part of n and ord_4(d) is the multiplicative
 * order of 4 modulo d (the smallest positive k with 4^k ≡ 1 (mod d)).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factorize_odd.h"

/* -------------------------------------------------------------------------- */
/* Utility: modular exponentiation for uint64_t arguments (safe from overflow) */
static seq_t
mod_pow_u64(seq_t base, uint64_t exp, seq_t mod)
{
    uint64_t b = (uint64_t)base % (uint64_t)mod;
    uint64_t r = 1;
    uint64_t m = (uint64_t)mod;
    while (exp > 0) {
        if (exp & 1ULL) {
            r = (unsigned __int128)r * b % m;
        }
        b = (unsigned __int128)b * b % m;
        exp >>= 1ULL;
    }
    return (seq_t)r;
}

/* -------------------------------------------------------------------------- */
/* Compute the multiplicative order of 4 modulo d.
 * The argument `phi_d` is φ(d) and is supplied by the caller.          */
static seq_t
order_of_4(seq_t d, seq_t phi_d)
{
    if (d == (seq_t)1) return 1;
    seq_t ord = phi_d;                     /* ord | φ(d) */

    /* factor φ(d) into distinct prime factors }
     * trial division up to sqrt(φ(d)).                                      */
    uint64_t tmp = (uint64_t)ord;
    uint64_t primes[32];
    int pcnt = 0;
    if (tmp % 2 == 0) {
        primes[pcnt++] = 2;
        while (tmp % 2 == 0) tmp /= 2;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[pcnt++] = tmp;

    /* reduce the order using the found prime factors */
    for (int i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        while (ord % p == 0 &&
               mod_pow_u64((seq_t)4, (uint64_t)(ord / p), d) == (seq_t)1) {
            ord /= p;
        }
    }
    return ord;
}

/* -------------------------------------------------------------------------- */
/* Recursive enumeration of all divisors of m.
 * cur_d   – current divisor being formed
 * cur_phi – φ(cur_d)   (kept in parallel)
 * sum     – accumulated result                        */
static void
gen_divs(int idx,
         seq_t cur_d,
         seq_t cur_phi,
         const idx_t *pf,
         const int *exps,
         int pf_cnt,
         seq_t *sum)
{
    if (idx == pf_cnt) {
        /* Contribution of the divisor cur_d */
        seq_t ord = order_of_4(cur_d, cur_phi);
        *sum += cur_phi / ord;
        return;
    }

    const idx_t p = pf[idx];
    const int  maxE = exps[idx];

    /* Pre‑compute powers of p and corresponding φ contributions       */
    seq_t p_pow[33];
    seq_t phi_con[33];
    p_pow[0] = (seq_t)1;
    phi_con[0] = (seq_t)1;
    for (int e = 1; e <= maxE; ++e) {
        p_pow[e] = p_pow[e - 1] * (seq_t)p;
        if (e == 1)
            phi_con[e] = (seq_t)(p - 1);
        else
            phi_con[e] = p_pow[e - 1] * (seq_t)(p - 1);
    }

    for (int e = 0; e <= maxE; ++e) {
        seq_t d2  = cur_d  * p_pow[e];
        seq_t phi2 = cur_phi * phi_con[e];
        gen_divs(idx + 1, d2, phi2, pf, exps, pf_cnt, sum);
    }
}

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --- 1. Obtain the odd part m of n ----------------------------------- */
    idx_t m = n;
    while ((m & 1ULL) == 0ULL) m >>= 1;

    if (m == 0) return 1;   /* harmless for n==0, though not in domain */

    /* --- 2. Factorise m ----------------------------------------------- */
    idx_t pf[32];
    int exp[32];
    int pf_cnt;
    factorize_odd(m, pf, exp, &pf_cnt);

    /* --- 3. Enumerate all divisors of m, summing the required quantity ---- */
    seq_t sum = 0;
    gen_divs(0, (seq_t)1, (seq_t)1, pf, exp, pf_cnt, &sum);

    return sum;
}
