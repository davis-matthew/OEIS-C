/*
 * A083344  a(n) = A082457(n) - A066715(n) = gcd(2n+1, A057643(2n+1)) -
 *          gcd(2n+1, A000203(2n+1)).
 * https://oeis.org/A083344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = gcd(2n+1, lcm{1 + d : d|2n+1}) - gcd(2n+1, sigma(2n+1))
 * Range:     i8 n <= 15, i16 n <= 51, i32 n <= 6506, i64 n <= 9993,
 *            i128 n <= 9993, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Recursively generate all divisors of a number given its prime factorization. */
static void gen_divisors_rec(const uint64_t *pr, const uint64_t *ex,
                             size_t idx, size_t cnt,
                             uint64_t current,
                             uint64_t *divs, size_t *div_cnt)
{
    if (idx == cnt) {
        divs[(*div_cnt)++] = current;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t pow = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors_rec(pr, ex, idx + 1, cnt,
                         current * pow, divs, div_cnt);
        pow *= p;
    }
}

/* Compute sigma(n) from its prime factorization. */
static seq_t compute_sigma(const uint64_t *pr, const uint64_t *ex, size_t fac_cnt)
{
    seq_t sigma = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        seq_t term = 1;
        seq_t p_pow = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) {
            p_pow *= (seq_t)pr[i];
            term += p_pow;
        }
        sigma *= term;
    }
    return sigma;
}

seq_t a(idx_t n)
{
    /* 2n+1 is odd; use 64‑bit integer to hold it safely. */
    uint64_t m = 2 * (uint64_t)n + 1;

    /* Factor m. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fac_cnt = 0;
    factor_u64(m, primes, exps, &fac_cnt);

    /* Sum of divisors sigma(m). */
    seq_t sigma = compute_sigma(primes, exps, fac_cnt);

    /* All divisors of m. */
    uint64_t divs[1024];
    size_t div_cnt = 0;
    gen_divisors_rec(primes, exps, 0, fac_cnt, 1, divs, &div_cnt);

    /* lcm of (1 + d) for all divisors d. */
    seq_t lcm_val = 1;
    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t term = (seq_t)(divs[i] + 1);
        seq_t g = gcd(lcm_val, term);
        lcm_val = lcm_val / g * term;
    }

    seq_t gcd1 = gcd((seq_t)m, lcm_val);
    seq_t gcd2 = gcd((seq_t)m, sigma);
    return gcd1 - gcd2;
}
