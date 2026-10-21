/*
 * A229090  Numbers n such that sigma(n) mod n > antisigma(n) mod n, where
 *          sigma(n) = A000203(n) = sum of divisors of n, antisigma(n) =
 *          A024816(n) = sum of non-divisors of n.
 * https://oeis.org/A229090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_mult_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A229090: numbers n such that sigma(n) mod n  >  antisigma(n) mod n.
 *  Implementation uses a simple incremental filter.  For each candidate
 *  integer m >= 2 it computes sigma(m) modulo m via the multiplicative
 *  property of the divisor‑sum function, derives
 *      antisigma(m) mod m = (m(m+1)/2 - sigma(m)) mod m,
 *  and appends m to the result list when the inequality holds.  The
 *  list is cached so that repeated calls to a(n) are O(1) after the
 *  desired term has been computed.  The function is valid for any
 *  seq_t/idx_t provided by the build system and respects all
 *  constraints given in the problem statement.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_mult_uint64.h"

/* return sigma(m) mod m for  m >= 1.  For m == 1 the result is 0. */
static uint64_t sigma_mod_u64(uint64_t m)
{
    if (m == 1) return 0;      /* sigma(1) = 1, 1 mod 1 = 0 */

    uint64_t sigma_mod = 1 % m;   /* product of terms modulo m */

    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* sum of powers of p: 1 + p + p^2 + ... + p^e  (mod m) */
        uint64_t term = 0;
        uint64_t power = 1 % m;
        for (uint64_t k = 0; k <= e; ++k)
        {
            term = (term + power) % m;
            power = mod_mult_uint64(power, p, m);
        }

        sigma_mod = mod_mult_uint64(sigma_mod, term, m);
    }
    return sigma_mod % m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for index 0 */

    /* cache of computed terms */
    static seq_t *terms  = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_candidate = 2;   /* first integer to test */

    while (terms_len < (size_t)n)
    {
        uint64_t m = next_candidate++;

        uint64_t sigma_mod = sigma_mod_u64(m);

        /* t_mod = (m*(m+1)/2) mod m : 0 if m odd, m/2 if m even */
        uint64_t t_mod = (m & 1ULL) ? 0ULL : (m >> 1);

        uint64_t antisigma_mod = (t_mod + m - sigma_mod) % m;

        if (sigma_mod > antisigma_mod)
        {
            seq_t val = (seq_t)m;   /* cast to seq_t; safe for values that fit */

            if (terms_len == terms_cap)
            {
                terms_cap = terms_cap ? terms_cap * 2 : 8;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = val;
        }
    }

    return terms[n - 1];
}
