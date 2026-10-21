/*
 * A228849  Number of distinct 12th powers modulo n.
 * https://oeis.org/A228849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   Number of distinct 12th powers modulo n. a(n) = ∏_{p^e || n}
 *            g(p^e) For an odd prime p, let φ(p^e)=p^{e-1}(p-1). For each t
 *            with 12t < e set e_t = e - 12t. The number of distinct residues
 *            of 12th powers modulo p^e is g(p^e) = 1 + Σ_{t≥0, 12t<e}
 *            φ(p^{e_t}) / gcd(12, φ(p^{e_t})). For p = 2 the unit...
 * Range:     u8 n <= 525, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow2.h"

static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* number of unit residues of x^12 modulo 2^e */
static seq_t unit_count_two(idx_t e)
{
    if (e <= 3)
        return (seq_t)1;
    /* e >= 4 */
    return pow2(e - 4);
}

/* number of distinct residues of x^12 modulo prime-power p^e */
static seq_t count_prime_power(idx_t p, idx_t e)
{
    seq_t sum_units = (seq_t)0;
    idx_t e_t = e;

    while (e_t > 0) {
        seq_t units;
        if (p == 2) {
            units = unit_count_two(e_t);
        } else {
            /* φ(p^e_t) = p^{e_t-1} * (p-1) */
            seq_t phi = (seq_t)(p - 1);
            if (e_t > 1)
                phi *= power_seq((seq_t)p, e_t - 1);
            seq_t d = gcd(phi, (seq_t)12);
            units = phi / d;
        }
        sum_units += units;

        /* next term: e_t -= 12; stop if e_t-12 <= 0 */
        if (e_t <= 12)
            break;
        e_t -= 12;
    }

    return (seq_t)1 + sum_units;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* factor n into distinct prime powers */
    idx_t primes[64];
    idx_t exps[64];
    int pc = 0;

    idx_t tmp = n;

    /* factor 2 */
    if ((tmp & (idx_t)1) == 0) {
        idx_t exp = 0;
        while ((tmp & (idx_t)1) == 0) {
            tmp >>= 1;
            ++exp;
        }
        primes[pc] = 2;
        exps[pc]  = exp;
        ++pc;
    }

    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p != 0)
            continue;
        idx_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        primes[pc] = p;
        exps[pc]  = exp;
        ++pc;
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[pc] = tmp;
        exps[pc]  = 1;
        ++pc;
    }

    seq_t result = (seq_t)1;
    for (int i = 0; i < pc; ++i)
        result *= count_prime_power(primes[i], exps[i]);

    return result;
}
