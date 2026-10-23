/*
 * A145634  Denominator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d
 *          for index l=2n+1 evaluated at x=13.
 * https://oeis.org/A145634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of A_{2n+1}(13) where * A_l(x) =
 *            sum_{d=1}^{l-1} x^{l-d}/d
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   mod_inv_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_inv_uint32.h"

/* integer power (small exponent, result fits in uint64_t) */
static uint64_t pow_uint64(uint64_t base, unsigned exp)
{
    uint64_t res = 1;
    while (exp--) res *= base;
    return res;
}

/* Helper: build prime list up to n (inclusive).  primes_len returned. */
static void build_primes(idx_t limit, idx_t **primes, size_t *primes_len)
{
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;

    *primes_len = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i])
    {
        ++(*primes_len);
    }
    *primes = (idx_t*)malloc((*primes_len) * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) (*primes)[idx++] = i;

    free(is_prime);
}

seq_t a(idx_t n)
{
    /*  l = 2n + 1  */
    idx_t l = 2 * n + 1;
    idx_t max_d = l - 1;           /* d runs from 1 .. max_d */

    /* --- prime factorisation up to max_d --- */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    if (max_d >= 2)
        build_primes(max_d, &primes, &primes_cnt);
    else
        primes_cnt = 0;

    /* exponent of each prime in lcm(1..max_d) */
    unsigned *e_vals = NULL;
    if (primes_cnt > 0)
        e_vals = (unsigned*)malloc(primes_cnt * sizeof(unsigned));
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p = primes[i];
        unsigned e = 0;
        idx_t pp = p;
        while (pp <= max_d)
        {
            ++e;
            if (pp > max_d / p) break;
            pp *= p;
        }
        e_vals[i] = e;
    }

    /* ---- main calculation: denominator by primes --- */
    seq_t denominator = 1;

    for (size_t ip = 0; ip < primes_cnt; ++ip)
    {
        idx_t p = primes[ip];
        unsigned e_p = e_vals[ip];
        uint64_t mod = pow_uint64(p, e_p);     /* m = p^e_p  (<= 19999) */

        /* Compute N_p = product over all other primes of q^e_q  (mod mod) */
        uint64_t N = 1;
        for (size_t iq = 0; iq < primes_cnt; ++iq)
        {
            if (iq == ip) continue;
            idx_t q = primes[iq];
            unsigned e_q = e_vals[iq];
            uint64_t pw = 1;
            for (unsigned k = 0; k < e_q; ++k)
                pw = (pw * (uint64_t)q) % mod;
            N = (N * pw) % mod;
        }

        /* inverses modulo mod */
        uint32_t *inv_mod = (uint32_t*)malloc(mod * sizeof(uint32_t));
        inv_mod[0] = 0;                   /* unused */
        for (uint32_t r = 1; r < mod; ++r)
            inv_mod[r] = mod_inv_uint32(r, (uint32_t)mod);

        /* pre‑compute p^k (mod mod) for k = 0..e_p   */
        uint32_t powp[15];                 /* e_p <= 13 for l <= 20000 */
        powp[0] = 1 % mod;
        for (unsigned k = 1; k <= e_p; ++k)
            powp[k] = (uint32_t)((powp[k-1] * (uint64_t)p) % mod);

        /* loop over d = max_d … 1 ------------------------------------- */
        uint64_t current_pow = 13 % mod;   /* corresponds to d = max_d */
        uint64_t sum = 0;

        for (idx_t d = max_d; d >= 1; --d)
        {
            /* exponent of p in d */
            idx_t dd = d;
            unsigned k = 0;
            while ((dd % p) == 0)
            {
                dd /= p;
                ++k;
            }
            /* d_prime = d / p^k  (coprime with mod) */
            uint64_t d_prime_mod = dd % mod;
            uint64_t inv_dp = inv_mod[(uint32_t)d_prime_mod];

            uint64_t factor = (N * inv_dp) % mod;
            factor = (factor * (uint64_t)powp[e_p - k]) % mod;

            sum += (current_pow * factor) % mod;
            if (sum >= mod) sum -= mod;

            /* prepare power for next iteration (decreasing d) */
            current_pow = (current_pow * 13) % mod;
        }

        /* valuation of sum at p */
        unsigned t = 0;
        uint64_t tmp = sum;
        if (tmp == 0)
            t = e_p;                          /* fully divisible by p^e_p */
        else
        {
            while ((tmp % p) == 0)
            {
                tmp /= p;
                ++t;
            }
        }

        unsigned rem = e_p - t;
        if (rem > 0)
        {
            seq_t mult = 1;
            for (unsigned ii = 0; ii < rem; ++ii)
                mult *= (seq_t)p;
            denominator *= mult;
        }

        free(inv_mod);
    }

    free(primes);
    free(e_vals);
    return denominator;
}
