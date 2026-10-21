/*
 * A054613  a(n) = Sum_{d|n} phi(d)*6^(n/d).
 * https://oeis.org/A054613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * 6^(n/d) (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

static void rec_div(uint64_t idx,
                    uint64_t cur_div,
                    uint64_t cur_phi,
                    seq_t *sum,
                    uint64_t n,
                    const uint64_t *pr,
                    const uint64_t *exp,
                    size_t cnt)
{
    if (idx == cnt) {
        seq_t exp_power = big_pow((seq_t)6, (seq_t)(n / cur_div));
        *sum += (seq_t)cur_phi * exp_power;
        return;
    }

    /* option: exponent 0 for this prime */
    rec_div(idx + 1, cur_div, cur_phi, sum, n, pr, exp, cnt);

    uint64_t p = pr[idx];
    uint64_t p_pow = 1;           /* will hold p^e */
    /* iterate exponent e from 1 .. exp[idx] */
    for (uint64_t e = 1; e <= exp[idx]; ++e) {
        cur_div *= p;
        p_pow *= p;               /* now p_pow = p^e */

        uint64_t phi_part;
        if (e == 1) {
            phi_part = p - 1;                         /* p^(0)*(p-1) */
        } else {
            phi_part = (p_pow / p) * (p - 1);        /* p^(e-1)*(p-1) */
        }
        rec_div(idx + 1, cur_div, cur_phi * phi_part,
                sum, n, pr, exp, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* factor n ----------------------------------------------------- */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            exps[cnt] = 0;
            while (m % p == 0) {
                m /= p;
                exps[cnt]++;
            }
            primes[cnt] = p;
            cnt++;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        cnt++;
    }

    /* compute the sum over all divisors ---------------------------- */
    seq_t sum = (seq_t)0;
    rec_div(0, (uint64_t)1, (uint64_t)1, &sum, (uint64_t)n, primes, exps, cnt);

    return sum;
}
