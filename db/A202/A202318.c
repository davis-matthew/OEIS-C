/*
 * A202318  Let (n)_p denote the exponent of prime p in the prime power
 *          factorization of n. Then a(n) is defined by the formulas a(1)=1;
 *          for n >= 2, (a(n))_2 = (n)_2, (a(n))_3 = (n)_3 and, for p >= 5,
 *          (a(n))_p = 1 + ((2n)/(p-1))_p if p-1|2*n, and (a(n))_p = 0
 *          otherwise.
 * https://oeis.org/A202318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 59, u64 n <= 269,
 *            u128 n <= 1259, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∏_{p} p^exp_p
 *   exp_2 = v_2(n)
 *   exp_3 = v_3(n)
 *   for p ≥ 5:
 *     if (p‑1) divides 2n then
 *         exp_p = 1 + v_p( (2n)/(p‑1) )
 *     else
 *         exp_p = 0
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

/* Recursively enumerate all divisors of t, processing each that
 * yields a prime p = d+1 (with p ≥ 5).  The result is accumulated in *res. */
static void recurse(const uint64_t *pr, const uint64_t *ex,
                    size_t idx, size_t cnt, idx_t cur, idx_t t,
                    seq_t *res)
{
    if (idx == cnt) {
        idx_t p = cur + 1;
        if (p >= 5 && is_prime(p)) {
            idx_t m = t / cur;
            idx_t exp_over = 0;
            while (m % p == 0) {
                m /= p;
                exp_over++;
            }
            size_t exp = exp_over + 1;
            for (size_t i = 0; i < exp; ++i)
                *res *= (seq_t)p;
        }
        return;
    }

    idx_t prod = 1;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        recurse(pr, ex, idx + 1, cnt, cur * prod, t, res);
        prod *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* exponent of 2 in n */
    while ((temp & 1) == 0) {
        result *= (seq_t)2;
        temp >>= 1;
    }

    /* exponent of 3 in n */
    while (temp % 3 == 0) {
        result *= (seq_t)3;
        temp /= 3;
    }

    idx_t t = n * 2;            /* 2n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factorize 2n */
    factor_u64(t, primes, exps, &cnt);

    /* enumerate all divisors of 2n and process */
    recurse(primes, exps, 0, cnt, (idx_t)1, t, &result);

    return result;
}
