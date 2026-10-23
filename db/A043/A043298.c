/*
 * A043298  Numbers n such that B(6*n) has denominator 42 where B(2k) are the
 *          Bernoulli numbers.
 * https://oeis.org/A043298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth integer m such that all primes p with p-1 | 6*m are
 *            exactly {2,3,7}. The development checks every integer m in
 *            increasing order, testing divisors of 6*m for the forbidden
 *            primes
 * Range:     u8 n <= 23, u16 n <= 5200, u32 n <= 8945, u64 n <= 8945,
 *            u128 n <= 8945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Integer types provided by the build */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* --------------------------------------------------------------------- */
/* Prime sieve up to a fixed bound                                          */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 2000000          /* large enough for n<=10000 */
static bool is_comp[SIEVE_LIMIT + 1];
static int primes[SIEVE_LIMIT / 10];
static int prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            primes[prime_cnt++] = i;
            if ((int64_t)i * i <= SIEVE_LIMIT)
            {
                for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* Small primality test using pre‑computed primes                         */
/* --------------------------------------------------------------------- */
static bool is_prime(uint64_t x)
{
    if (x <= (uint64_t)SIEVE_LIMIT)
        return x >= 2 && !is_comp[(int)x];
    if (x < 4) return x == 2 || x == 3;
    for (int i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Factorisation of a 64‑bit integer into arrays of primes and exponents   */
/* --------------------------------------------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *pf, int *exp, int *cnt)
{
    *cnt = 0;
    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i)
    {
        uint64_t p = (uint64_t)primes[i];
        if (n % p == 0)
        {
            int e = 0;
            do
            {
                n /= p;
                ++e;
            } while (n % p == 0);
            pf[(*cnt)] = p;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1)
    {
        pf[(*cnt)] = n;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* --------------------------------------------------------------------- */
/* Recursively enumerate all divisors of N based on its factorisation      */
/* --------------------------------------------------------------------- */
static bool bad_divisor_found;
static void gen_divisors(const uint64_t *pf, const int *exp, int tot, int idx,
                         uint64_t cur)
{
    if (bad_divisor_found) return;
    if (idx == tot)
    {
        uint64_t p = cur + 1ULL;
        if (p != 2 && p != 3 && p != 7 && is_prime(p))
            bad_divisor_found = true;
        return;
    }
    uint64_t mult = 1;
    for (int e = 0; e <= exp[idx]; ++e)
    {
        gen_divisors(pf, exp, tot, idx + 1, cur * mult);
        mult *= pf[idx];
    }
}

/* --------------------------------------------------------------------- */
/* Check whether m satisfies the condition                                */
/* --------------------------------------------------------------------- */
static bool good_candidate(uint64_t m)
{
    uint64_t N = 6ULL * m;
    uint64_t pf[32];
    int exp[32];
    int cnt;
    factor_uint64(N, pf, exp, &cnt);

    bad_divisor_found = false;
    gen_divisors(pf, exp, cnt, 0, 1ULL);  /* start with divisor 1 */
    return !bad_divisor_found;
}

/* --------------------------------------------------------------------- */
/* Dynamic cache for generated sequence                                   */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t next_candidate = 1;   /* next integer to test for m */

static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out-of‑range, defined as 0 */

    init_primes();          /* initialise primes once */

    if ((size_t)n > cache_size)
    {
        while (cache_size < (size_t)n)
        {
            while (!good_candidate((uint64_t)next_candidate))
                ++next_candidate;

            ensure_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
