/*
 * A214028  Entry points for the Pell sequence: smallest k such that n divides
 *          A000129(k).
 * https://oeis.org/A214028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides Pell(k) (A000129(k)). The
 *            algorithm uses the factorisation of n. For prime powers a(2^e) =
 *            2^e. For an odd prime p, a(p^e) = p^max(0,e-s) * a(p), where
 *            a(p) = least k with p | Pell(k) (k ≤ p-(8/p)) and s = v_p(
 *            Pell(a(p)) )
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, legendre_two, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary definitions */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"
#include "../utils/legendre_two.h"

/* Least common multiple for seq_t values. */
static inline seq_t lcm_seq_t(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return seq_mul(a / g, b);
}

/* For an odd prime p and exponent e, compute
   a(p) and s = v_p( Pell(a(p)) ). */
static inline void ap_and_s(uint64_t p, uint64_t e, seq_t *ap_out, uint64_t *s_out)
{
    int l = legendre_two(p);
    uint64_t t = p - (uint64_t)l;          /* upper bound for k */

    seq_t mod_p  = (seq_t)p;
    seq_t mod_pe = pow_uint(p, e);          /* p^e */

    seq_t prev_p  = 0, cur_p  = 1;
    seq_t prev_pe = 0, cur_pe = 1;
    seq_t next_p, next_pe;

    /* k = 1 already checked; start from k = 2 */
    for (uint64_t k = 2; k <= t; ++k)
    {
        next_p  = (2 * cur_p + prev_p) % mod_p;
        prev_p  = cur_p;
        cur_p   = next_p;

        next_pe = (2 * cur_pe + prev_pe) % mod_pe;
        prev_pe = cur_pe;
        cur_pe  = next_pe;

        if (cur_p == 0)            /* found a(p) */
        {
            *ap_out = (seq_t)k;
            /* valuation of Pell(k) modulo p^e */
            seq_t val = cur_pe;
            uint64_t s = 0;
            while (s < e && val % (seq_t)p == 0)
            {
                val /= (seq_t)p;
                ++s;
            }
            *s_out = s;
            return;
        }
    }
    /* Fallback – should never happen for the tested range. */
    *ap_out = (seq_t)t;
    *s_out  = 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part;

        if (p == 2)
        {
            /* a(2^e) = 2^e */
            part = pow_uint(2, e);
        }
        else
        {
            seq_t ap;
            uint64_t s;
            ap_and_s(p, e, &ap, &s);

            uint64_t extra = (e > s) ? (e - s) : 0;
            if (extra == 0)
                part = ap;
            else
                part = seq_mul(ap, pow_uint(p, extra));
        }

        result = lcm_seq_t(result, part);
    }

    return result;
}
