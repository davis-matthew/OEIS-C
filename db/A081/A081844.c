/*
 * A081844  Number of irreducible factors of x^(2n+1) - 1 over GF(2).
 * https://oeis.org/A081844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{ d|2n+1 } φ(d)/ord_2(d) * where φ is Euler's totient
 *            and ord_2(d) is the multiplicative order * of 2 modulo d. This
 *            is the number of cycles of the doubling map * x → 2x (mod
 *            (2n+1)). The code factorises 2n+1 and generates all * its
 *            divisors iteratively. All arithmetic stays within 64‑...
 * Range:     u8 n <= 2046, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_overflow_u64, ord_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_overflow_u64.h"
#include "../utils/ord_2.h"

/* Factorisation of n (≥1) into prime factors.
 * primes[] and exps[] store the factors; *cnt is updated.
 */
static void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt) {
    size_t i = 0;
    uint64_t d = n;
    for (uint64_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            uint64_t e = 0;
            while (d % p == 0) {
                d /= p;
                ++e;
            }
            primes[i] = p;
            exps[i] = e;
            ++i;
        }
    }
    if (d > 1) {
        primes[i] = d;
        exps[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* Recursive generation of divisors of the number represented by primes[0..cnt-1]^(exps) .
 * cur   : current divisor being built
 * curPhi: Euler's totient of current divisor
 * pos   : current factor index
 * sum   : accumulated result (modulo of phi/ord_2 added)
 */
static void dfs_divs(size_t pos, uint64_t cur, uint64_t curPhi,
                     const uint64_t *primes, const uint64_t *exps, size_t cnt,
                     seq_t *sum) {
    if (pos == cnt) {
        uint64_t ord = ord_2(cur);
        *sum += (seq_t)(curPhi / ord);
        return;
    }
    /* e = exponent chosen for this prime: 0 .. exps[pos] */
    uint64_t p = primes[pos];
    uint64_t pe = 1;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        /* phi contribution: if e==0 => *1; else p^(e-1)*(p-1) */
        uint64_t newPhi = curPhi;
        if (e > 0) {
            // multiply by p^(e-1)*(p-1)
            for (uint64_t i = 0; i < e - 1; ++i) {
                newPhi *= p;
            }
            newPhi *= (p - 1);
        }
        dfs_divs(pos + 1, cur * pe, newPhi,
                 primes, exps, cnt, sum);
        pe *= p;
    }
}

seq_t a(idx_t n) {
    /* Compute a(n) for n >= 0 */
    seq_t result = 0;
    /* m = 2n+1, always odd */
    uint64_t m = ((uint64_t)n * 2) + 1;
    if (m == 1) { /* n==0 case */
        return (seq_t)1;
    }
    /* Factorise m */
    uint64_t primes[20], exps[20];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);
    /* Generate all divisors, compute phi and accumulate */
    dfs_divs(0, 1, 1, primes, exps, cnt, &result);
    return result;
}
