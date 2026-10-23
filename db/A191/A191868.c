/*
 * A191868  Numbers n such that (product of divisors of 2n) mod (sum of
 *          divisors of 2n) is equal to number of divisors of 2n.
 * https://oeis.org/A191868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) is the nth positive integer such that (product of divisors
 *            of 2x) mod (sum of divisors of 2x) equals the number of divisors
 *            of 2x (τ(2x)). The sequence begins 1,4,7,13,19,... as in OEIS
 *            A191868
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t *spf = NULL;           /* smallest prime factor array */
static const uint32_t SPF_LIMIT = 4000000U;   /* upper bound for SPF array */

/* ------------------------------------------------------------------ */
/* Compute smallest prime factor array up to SPF_LIMIT.               */
/* ------------------------------------------------------------------ */
static void init_spf(void)
{
    if (spf != NULL)
        return;

    spf = (uint32_t *)malloc((size_t)(SPF_LIMIT + 1) * sizeof(uint32_t));
    if (!spf)
        return;

    for (uint32_t i = 0; i <= SPF_LIMIT; ++i)
        spf[i] = 0;
    spf[0] = spf[1] = 1;

    for (uint32_t i = 2; (uint64_t)i * i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            for (uint64_t j = (uint64_t)i * i; j <= SPF_LIMIT; j += i) {
                if (spf[(uint32_t)j] == 0)
                    spf[(uint32_t)j] = i;
            }
        }
    }
    for (uint32_t i = 2; i <= SPF_LIMIT; ++i)
        if (spf[i] == 0)
            spf[i] = i;              /* remaining primes */
}

/* ------------------------------------------------------------------ */
/* Modular exponentiation: compute (base^exp) mod modv.                 */
/* ------------------------------------------------------------------ */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t modv)
{
    uint64_t result = 1 % modv;
    base %= modv;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % modv;
        base = (base * base) % modv;
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* test whether candidate x satisfies the property for 2x.           */
/* ------------------------------------------------------------------ */
static bool satisfies(uint64_t x)
{
    uint64_t m = 2ULL * x;          /* m = 2*x */
    uint64_t temp = m;
    uint64_t tau = 1;               /* number of divisors of m */
    uint64_t sigma = 1;             /* sum of divisors of m */
    uint64_t sqrt_m = 1;            /* sqrt(m) if m is a perfect square */

    /* factorize m using SPF */
    while (temp > 1) {
        uint32_t p = spf[(uint32_t)temp];
        uint64_t cnt = 0;
        uint64_t p_pow = 1;
        uint64_t p_pow_sq = 1;      /* p^(cnt/2) for sqrt_m */

        while (temp % p == 0) {
            temp /= p;
            ++cnt;
            p_pow *= p;
        }

        tau *= (cnt + 1);

        /* sum of powers of p */
        uint64_t sum_powers = 1;    /* p^0 */
        uint64_t pow_now = 1;
        for (uint64_t i = 0; i < cnt; ++i) {
            pow_now *= p;
            sum_powers += pow_now;
        }
        sigma *= sum_powers;

        /* prepare sqrt_m if needed */
        if (cnt % 2 == 0) {
            /* multiply by p^(cnt/2) */
            uint64_t p_half = 1;
            for (uint64_t i = 0; i < cnt / 2; ++i)
                p_half *= p;
            sqrt_m *= p_half;
        }
    }

    /* compute m^(tau/2) mod sigma */
    uint64_t exp_half = tau >> 1;   /* floor(tau/2) */
    uint64_t prod_mod = mod_pow_uint64(m, exp_half, sigma);

    if (tau & 1ULL) {               /* tau odd: multiply by sqrt(m) */
        prod_mod = (prod_mod * sqrt_m) % sigma;
    }

    return prod_mod == tau;
}

/* ------------------------------------------------------------------ */
/* Storage of the sequence values.                                   */
/* ------------------------------------------------------------------ */
static seq_t *seq = NULL;           /* array of terms */
static idx_t seq_len = 0;           /* number of terms known */
static idx_t seq_cap = 0;           /* allocated capacity */
static uint64_t next_candidate = 1; /* next integer to test */

/* ensure capacity for sequence array */
static void ensure_seq_capacity(idx_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure; leave as is */
    seq = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function to return the nth term of the sequence.             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n<1 */

    init_spf();

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        if (satisfies(next_candidate)) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
