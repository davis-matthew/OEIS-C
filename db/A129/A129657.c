/*
 * A129657  Infinitary deficient numbers: integers for which A126168(n) < n,
 *          or equivalently for which A049417(n) < 2n.
 * https://oeis.org/A129657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer m such that the sum of its infinitary
 *            divisors σ_inf(m) < 2m. The function tests successive integers,
 *            factors them with a trial‑division routine, computes σ_inf(m)
 *            from the factorisation and counts the numbers for which the
 *            inequality holds
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
 * Basic utilities
 * ------------------------------------------------------------------ */

/* Maximum number of distinct prime factors needed for the limits
   considered. 20 is more than enough for m up to ≈12000. */
#define MAX_FACTORS 20

/* Test if an unsigned 64‑bit integer is a power of two.  Used to
   compute p^(2^i). */
static bool is_power_of_two_uint64(uint64_t n)
{
    return n && !(n & (n - 1));
}

/* ------------------------------------------------------------------
 * Small prime sieve (once per program run)
 * ------------------------------------------------------------------ */
static bool _primes_initialised = false;
static uint64_t _primes[3000];
static size_t _num_primes;

/* Initialise a list of primes up to 20000 (sufficient for
   m≈12000 required for n≤10000). */
static void _init_primes(void)
{
    if (_primes_initialised)
        return;
    const uint64_t LIM = 20000;
    bool is_composite[LIM + 1];
    for (uint64_t i = 2; i <= LIM; ++i)
        is_composite[i] = false;
    for (uint64_t i = 2; i * i <= LIM; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= LIM; j += i)
                is_composite[j] = true;
        }
    }
    _num_primes = 0;
    for (uint64_t i = 2; i <= LIM; ++i) {
        if (!is_composite[i])
            _primes[_num_primes++] = i;
    }
    _primes_initialised = true;
}

/* ------------------------------------------------------------------
 * Factorisation by trial division
 * ------------------------------------------------------------------ */
static void _factor_uint64(uint64_t n, uint64_t *factors, uint64_t *exps,
                           size_t *cnt)
{
    *cnt = 0;
    if (n < 2)
        return;
    _init_primes();
    for (size_t i = 0; i < _num_primes && _primes[i] * _primes[i] <= n; ++i) {
        uint64_t p = _primes[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            factors[(*cnt)] = p;
            exps[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        factors[(*cnt)] = n;
        exps[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------
 * Small integer power for seq_t (exponent < 32768)
 * ------------------------------------------------------------------ */
static seq_t _pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    for (unsigned int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* ------------------------------------------------------------------
 * Main sequence generator
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* n≥1 by definition; this is just safe */

    uint64_t m = 1;
    uint64_t found = 0L;

    uint64_t factors[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t fact_cnt;

    while (found < n) {
        _factor_uint64(m, factors, exps, &fact_cnt);

        /* compute σ_inf(m) */
        seq_t sigma = (seq_t)1;
        seq_t limit = (seq_t)m * (seq_t)2;
        bool overflow = false;

        for (size_t i = 0; i < fact_cnt && !overflow; ++i) {
            uint64_t p = factors[i];
            uint64_t exp = exps[i];
            unsigned int bit = 0;

            /* iterate over bits of exp */
            while (exp && !overflow) {
                if (exp & 1ULL) {
                    /* compute p^(2^bit) */
                    unsigned int pow_exp = 1U << bit;      /* 2^bit */
                    seq_t p_pow = _pow_seq((seq_t)p, pow_exp);
                    seq_t term = p_pow + (seq_t)1;

                    sigma *= term;
                    if (sigma > limit)
                        overflow = true;
                }
                exp >>= 1;
                ++bit;
            }
        }

        if (!overflow && sigma < limit) {
            ++found;
            if (found == n)
                return (seq_t)m;
        }
        ++m;
    }

    /* Should never reach here */
    return 0;
}
