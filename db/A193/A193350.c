/*
 * A193350  Sum of even divisors of tau(n).
 * https://oeis.org/A193350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of even divisors of τ(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- simple prime sieve for n ≤ 10000 ---------- */
static uint64_t primes[1229];          /* number of primes ≤ 10000 */
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    const uint64_t N = 10000;          /* limit sufficient for factoring n ≤ 10000 */
    bool sieve[N + 1];
    for (size_t i = 0; i <= N; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (uint64_t i = 2; i <= N; ++i) {
        if (sieve[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= N)
                for (uint64_t j = i * i; j <= N; j += i)
                    sieve[j] = false;
        }
    }
    primes_ready = true;
}

/* factorise n, storing primes and exponents in the supplied arrays.
   Assumes arrays are large enough (≥10 for n ≤ 10000). */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    init_primes();
    size_t j = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            uint64_t e = 0;
            while (n % primes[i] == 0) {
                n /= primes[i];
                ++e;
            }
            pr[j] = primes[i];
            exp[j] = e;
            ++j;
        }
    }
    if (n > 1) {                /* remaining prime factor */
        pr[j] = n;
        exp[j] = 1;
        ++j;
    }
    *cnt = j;
}

/* compute Σ(d) for all divisors d of m given its prime factorisation */
static seq_t sigma_from_factors(const uint64_t *pr, const uint64_t *exp, size_t cnt)
{
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t sum = 1;
        seq_t pow = 1;
        for (uint64_t e = 0; e < exp[i]; ++e) {
            pow *= p;    /* p^(e+1) after increment */
            sum += pow;
        }
        result *= sum;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n ≤ 0, guard */
    uint64_t nin = (uint64_t)n;

    /* factor n to compute τ(n) */
    uint64_t pr_n[20], exp_n[20];
    size_t cnt_n = 0;
    factor_uint64(nin, pr_n, exp_n, &cnt_n);

    seq_t tau = 1;
    for (size_t i = 0; i < cnt_n; ++i)
        tau *= (seq_t)(exp_n[i] + 1);

    /* if τ(n) is odd, there are no even divisors */
    if ((tau & 1) == 1) return 0;

    uint64_t tau_u = (uint64_t)tau;

    /* factor τ(n) to compute σ(τ(n)) and σ_odd(τ(n)) */
    uint64_t pr_t[20], exp_t[20];
    size_t cnt_t = 0;
    factor_uint64(tau_u, pr_t, exp_t, &cnt_t);

    seq_t sigma_all = sigma_from_factors(pr_t, exp_t, cnt_t);

    /* compute σ of odd part: skip factor 2 */
    seq_t sigma_odd = 1;
    bool has_odd = false;
    for (size_t i = 0; i < cnt_t; ++i) {
        if (pr_t[i] == 2) continue;     /* skip 2 */
        has_odd = true;
        seq_t p = (seq_t)pr_t[i];
        seq_t sum = 1;
        seq_t pow = 1;
        for (uint64_t e = 0; e < exp_t[i]; ++e) {
            pow *= p;
            sum += pow;
        }
        sigma_odd *= sum;
    }
    if (!has_odd) sigma_odd = 1;           /* if τ(n) is a power of 2 */

    return sigma_all - sigma_odd;          /* sum of even divisors of τ(n) */
}
