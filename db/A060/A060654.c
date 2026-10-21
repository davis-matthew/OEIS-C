/*
 * A060654  a(n) = gcd(n, A060766(n)).
 * https://oeis.org/A060654
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1884, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sort_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A060654: a(n) = gcd(n, lcm(dd(n))) where dd(n) is the set of distinct differences between consecutive divisors of n.
   Offset: 2 (a(2) is the first term) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/sort_seq_t.h"

/* Prime factorisation of a 64‑bit unsigned integer.
   Works for both 64‑bit and big‑int builds; the result is always
   small enough to fit in uint64_t arrays. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    if (n == 0) return;

    /* factor 2s */
    if ((n & 1ULL) == 0) {
        uint64_t e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        primes[(*cnt)] = 2;
        exps[(*cnt)]   = e;
        ++(*cnt);
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* Recursive generation of divisors.
   Parameters:
     idx   – current prime exponent index
     primes, exps, cnt – factorisation data
     cur   – product built so far
     out   – array to fill (pre‑allocated)
     len   – pointer to current length; incremented when a divisor is added */
static void gen_divs_rec(size_t idx,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         size_t cnt,
                         seq_t cur,
                         seq_t *out,
                         size_t *len)
{
    if (idx == cnt) {
        out[(*len)++] = cur;
        return;
    }
    seq_t t = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(idx + 1, primes, exps, cnt, t, out, len);
        t = t * (seq_t)primes[idx];
    }
}

static void gen_divisors(const uint64_t *primes,
                         const uint64_t *exps,
                         size_t cnt,
                         seq_t *out,
                         size_t *len)
{
    *len = 0;
    gen_divs_rec(0, primes, exps, cnt, (seq_t)1, out, len);
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* Sequence is defined for n >= 2 */

    /* Factorise n */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[20];
    uint64_t exps[20];
    size_t fc;
    factor_uint64(nn, primes, exps, &fc);

    /* Compute number of divisors */
    size_t div_cap = 1;
    for (size_t i = 0; i < fc; ++i) {
        div_cap *= (exps[i] + 1);
    }

    /* Allocate a static array large enough for all 64‑bit numbers */
    /* 103680 is the known maximum divisor count for 64‑bit integers. */
    static seq_t divs[110000];
    size_t dlen;

    /* Generate all divisors */
    gen_divisors(primes, exps, fc, divs, &dlen);

    /* Sort the divisors */
    sort_seq_t(divs, dlen);

    /* Compute LCM of the distinct consecutive differences */
    seq_t lcm_val = (seq_t)1;
    for (size_t i = 1; i < dlen; ++i) {
        seq_t diff = divs[i] - divs[i - 1];
        seq_t g    = gcd(lcm_val, diff);
        seq_t tmp  = lcm_val / g;
        lcm_val    = tmp * diff;
    }

    /* Result is the gcd with n */
    seq_t result = gcd((seq_t)nn, lcm_val);
    return result;
}
