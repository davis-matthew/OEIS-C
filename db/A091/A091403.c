/*
 * A091403  Numbers n such that genus of group Gamma_0(n) is 1.
 * https://oeis.org/A091403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   compute the nth term of A091403 by searching for the smallest
 *            integers n with genus of Γ₀(n) equal to 1
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: full, pass, 12 terms through n = 12 (2026-11-02)
 * Helpers:   build_primes_upto, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"

/*  Recursive enumeration of all divisors of N from its factorisation.
    cur_exp holds the current exponent choice for each prime.
    sum receives Σ φ(gcd(d,N/d)) over all divisors d.  */
static void recur_divisors(const idx_t *p,
                           const uint8_t *e,
                           size_t cnt,
                           uint8_t *cur_exp,
                           seq_t *sum,
                           size_t depth)
{
    if (depth == cnt) {
        seq_t phi = 1;
        for (size_t j = 0; j < cnt; ++j) {
            uint8_t a = cur_exp[j];
            uint8_t b = e[j] - a;
            uint8_t ge = a < b ? a : b;
            if (ge > 0) {
                seq_t pw = 1;
                for (uint8_t k = 0; k < ge - 1; ++k)
                    pw *= (seq_t)p[j];
                seq_t term = pw * (seq_t)(p[j] - 1);
                phi *= term;
            }
        }
        *sum += phi;
        return;
    }
    for (uint8_t exp = 0; exp <= e[depth]; ++exp) {
        cur_exp[depth] = exp;
        recur_divisors(p, e, cnt, cur_exp, sum, depth + 1);
    }
}

/*  Prime table (uint64_t) used for factorisation.  */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/*  Ensure we have all primes <= limit.  If the table already covers
    the required limit, nothing is done.  */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
        free(prime_list);
    /*  build_primes_upto expects a uint64_t**; cast is safe.  */
    build_primes_upto(limit, (uint64_t **)&prime_list, &prime_cnt);
    prime_limit = limit;
}

/*  True iff genus of Γ₀(N) equals 1.  The calculation follows the
    classical formula (see the OEIS entry), using signed __int128 for
    intermediate arithmetic to avoid underflow.  */
static bool genus_is_one(idx_t N)
{
    /*  Factorise N.  */
    idx_t fac_p[32];
    uint8_t fac_e[32];
    size_t fac_cnt = 0;

    idx_t tmp = N;
    idx_t sq = isqrt(tmp);
    ensure_primes_upto(sq);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= sq; ++i) {
        idx_t p = prime_list[i];
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac_p[fac_cnt] = p;
            fac_e[fac_cnt] = e;
            ++fac_cnt;
            sq = isqrt(tmp);
        }
    }
    if (tmp > 1) {          /* remaining prime factor > sqrt(original N) */
        fac_p[fac_cnt] = tmp;
        fac_e[fac_cnt] = 1;
        ++fac_cnt;
    }

    /*  μ = N * ∏ (p+1)/p.  */
    __int128 mu = N;
    for (size_t i = 0; i < fac_cnt; ++i) {
        idx_t p = fac_p[i];
        mu = (mu * (__int128)(p + 1)) / p;
    }

    /*  e2.  */
    __int128 e2 = 1;
    if (N % 4 == 0)
        e2 = 0;
    else {
        for (size_t i = 0; i < fac_cnt; ++i) {
            idx_t p = fac_p[i];
            if (p == 2)
                continue;
            if (p % 4 == 1)
                e2 *= 2;
            else if (p % 4 == 3) {
                e2 = 0;
                break;
            }
        }
    }

    /*  e3.  */
    __int128 e3 = 1;
    if (N % 9 == 0)
        e3 = 0;
    else {
        for (size_t i = 0; i < fac_cnt; ++i) {
            idx_t p = fac_p[i];
            if (p == 3)
                continue;
            if (p % 3 == 1)
                e3 *= 2;
            else if (p % 3 == 2) {
                e3 = 0;
                break;
            }
        }
    }

    /*  number of cusps: Σ φ(gcd(d,N/d)).  */
    seq_t cusps_seq = 0;
    uint8_t cur_exp[32];
    recur_divisors(fac_p, fac_e, fac_cnt, cur_exp, &cusps_seq, 0);
    __int128 cusps = (__int128)cusps_seq;

    /*  Compute numerator of 12*g.  */
    __int128 num = 12 + mu - 3 * e2 - 4 * e3 - 6 * cusps;
    __int128 g = num / 12;
    return g == 1;
}

/*  Return the nth element of A091403 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    idx_t candidate = 1;

    while (1) {
        if (genus_is_one(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
