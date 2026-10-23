/*
 * A178109  The maximum number d, 2 <= d <= n/2, which divides
 *          binomial(n-d-1,d-1) and is not coprime to n.
 * https://oeis.org/A178109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = maximum integer d, 2 ≤ d ≤ n/2, such that d divides
 *            C(n−d−1, d−1) and gcd(d,n) > 1; otherwise a(n) = 0
 * Range:     u8 n <= 525, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Euclidean GCD for 64‑bit unsigned integers. */
static idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Exponent of prime p in n! (Legendre's formula). */
static idx_t exp_in_fact(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* Compute the sequence value for index n. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* No divisor in the allowed range. */

    const idx_t dmax = n / 2;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pf_cnt;

    /* Search d from largest to smallest to get the maximum. */
    for (idx_t d = dmax; d >= 2; --d) {
        /* Skip d that is coprime to n. */
        if (gcd_uint64(d, n) == 1)
            continue;

        /* Parameters of the binomial coefficient C(m, k). */
        idx_t m = n - d - 1;
        idx_t k = d - 1;

        /* Factor d into primes. */
        factor_u64((uint64_t)d, primes, exps, &pf_cnt);

        int ok = 1;
        for (size_t i = 0; i < pf_cnt; ++i) {
            idx_t p = primes[i];
            idx_t required = exps[i];
            idx_t available = exp_in_fact(m, p)
                            - exp_in_fact(k, p)
                            - exp_in_fact(m - k, p);
            if (available < required) {
                ok = 0;
                break;
            }
        }

        if (ok)
            return (seq_t)d;       /* Found the maximum d. */
    }

    return 0;                    /* No suitable divisor found. */
}
