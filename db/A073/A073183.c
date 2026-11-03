/*
 * A073183  Sum of divisors of n that are not greater than the cubefree kernel
 *          of n.
 * https://oeis.org/A073183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9864 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073183:  a(n) = sum of the divisors of n not exceeding its cubefree kernel
 *          (product of the prime powers p^min(e,2) in the factorization of n)  */
#include <stdint.h>
#include <stddef.h>

/* Recursive summation of all divisors d of n with d ≤ limit.  limit is
 * a 64‑bit value; any divisor larger than the 64‑bit maximum is automatically
 * ignored.  The products are performed in 64‑bit arithmetic to avoid overflow. */
static void sum_divisors_limit(const uint64_t *pr, const uint64_t *exp, size_t cnt,
                               size_t idx, seq_t cur, uint64_t limit, seq_t *sum)
{
    if (idx == cnt) {
        if ((uint64_t)cur <= limit)
            *sum += cur;
        return;
    }
    uint64_t prod = 1;                     /* p^e   for current prime */
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        /* cur * prod must not exceed limit.  Use 64‑bit division to test safely. */
        if ((uint64_t)cur > limit / prod)
            break;                       /* any larger e would be even larger */

        uint64_t new_val = (uint64_t)cur * prod;
        if (new_val <= limit)
            sum_divisors_limit(pr, exp, cnt,
                               idx + 1, (seq_t)new_val, limit, sum);

        if (e == exp[idx])                 /* no higher exponent possible */
            break;

        /* Prepare prod for the next exponent, guarding against overflow beyond
         * the 64‑bit limit.  If prod would exceed limit, future products
         * (and thus divisors) will all be too large.  */
        if (prod > limit / pr[idx])
            prod = limit + 1;
        else
            prod *= pr[idx];
    }
}

/* --------------------------------------------------------------------- */
/* Simple trial‑division factorization.  Assumes n > 0.  Prime factors are
 * stored in pr[] with corresponding exponents in exp[], and the number of
 * distinct primes in *cnt.  Only the first 32 prime powers are stored.     */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    uint64_t t = n;
    /* factor 2 */
    if (t % 2 == 0) {
        uint64_t e = 0;
        do { t /= 2; ++e; } while (t % 2 == 0);
        pr[(*cnt)] = 2;
        exp[(*cnt)] = e;
        ++(*cnt);
    }
    /* odd primes */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            uint64_t e = 0;
            do { t /= p; ++e; } while (t % p == 0);
            pr[(*cnt)] = p;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    /* remaining prime */
    if (t > 1) {
        pr[(*cnt)] = t;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                      /* sequence is defined for n ≥ 1; return 0 for safety */
        return 0;

    /* Factorise n */
    uint64_t pr[32];
    uint64_t exp[32];
    size_t   cnt;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    /* Compute cubefree kernel limit = ∏ p^min(e,2) */
    uint64_t limit = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exp[i] < 2 ? exp[i] : 2;
        for (uint64_t k = 0; k < e; ++k) {
            if (limit > UINT64_MAX / pr[i])
                limit = UINT64_MAX;
            else
                limit *= pr[i];
        }
        if (limit == UINT64_MAX)
            break;
    }

    /* Sum all divisors d of n with d ≤ limit */
    seq_t sum = 0;
    sum_divisors_limit(pr, exp, cnt, 0, (seq_t)1, limit, &sum);
    return sum;
}
