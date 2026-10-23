/*
 * A045814  Number of times the digits are repeated in A045813.
 * https://oeis.org/A045814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = common digit count among the base‑4 representations of
 *            all divisors of the n‑th term of sequence A045813. Each term of
 *            A045813 is defined implicitly as the smallest integer for which
 *            this common count exists; a(n) is that count
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* Constants */
#define MAX_CACHE 200000          /* maximum number of a(n) values to cache  */
#define MAX_PRIME 2000000         /* sieve primes up to this limit       */
#define MAX_DIVISORS 8192         /* maximum divisors a tested number can have */

/* Static tables and counters */
static seq_t cache[MAX_CACHE + 1] = {0};
static size_t cache_len = 1;          /* index 0 is unused                           */
static uint64_t last_checked = 1;     /* last integer examined for property          */

static int primes[MAX_PRIME];
static size_t primes_cnt = 0;

/* Initialise prime table once (sieve of Eratosthenes) */
static void init_primes(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* Allocate temporary boolean array */
    unsigned char *is_prime = (unsigned char *)malloc((MAX_PRIME + 1) * sizeof(unsigned char));
    if (!is_prime) exit(1);
    for (int i = 0; i <= MAX_PRIME; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i * i <= MAX_PRIME; ++i)
        if (is_prime[i])
            for (long long j = 1LL * i * i; j <= MAX_PRIME; j += i)
                is_prime[(size_t)j] = 0;
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) primes[primes_cnt++] = i;
    free(is_prime);
}

/* Factorise n into primes (primes array must contain all primes up to sqrt(n))
   fac_p    : array of prime factors
   fac_exp  : corresponding exponents
   fac_cnt  : number of distinct prime factors
   The arrays must be long enough. */
static void factor_uint64(uint64_t n,
                          uint64_t fac_p[32],
                          int fac_exp[32],
                          size_t *fac_cnt)
{
    *fac_cnt = 0;
    uint64_t temp = n;
    for (size_t idx = 0; idx < primes_cnt && (uint64_t)primes[idx] * (uint64_t)primes[idx] <= temp; ++idx)
    {
        if (temp % primes[idx] == 0)
        {
            int e = 0;
            uint64_t p = primes[idx];
            do { temp /= p; ++e; } while (temp % p == 0);
            fac_p[(*fac_cnt)] = p;
            fac_exp[(*fac_cnt)] = e;
            ++(*fac_cnt);
        }
    }
    if (temp > 1)
    {
        fac_p[(*fac_cnt)] = temp;
        fac_exp[(*fac_cnt)] = 1;
        ++(*fac_cnt);
    }
}

/* Generate all divisors from prime factorisation */
static void generate_divisors(const uint64_t *p, const int *e, size_t cnt,
                              uint64_t *divs, size_t *div_cnt)
{
    divs[0] = 1;
    *div_cnt = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t prime = p[i];
        int exp = e[i];
        size_t cur = *div_cnt;
        uint64_t mul = 1;
        for (int k = 1; k <= exp; ++k)
        {
            mul *= prime;
            for (size_t j = 0; j < cur; ++j)
                divs[(*div_cnt)++] = divs[j] * mul;
        }
    }
}

/* Test whether the number m satisfies the A045814 property.
   If true, *common represents the equal digit count. */
static int test_number(uint64_t m, unsigned long long *common)
{
    uint64_t fac_p[32];
    int fac_exp[32];
    size_t fac_cnt;
    factor_uint64(m, fac_p, fac_exp, &fac_cnt);

    uint64_t divisors[MAX_DIVISORS];
    size_t div_cnt;
    generate_divisors(fac_p, fac_exp, fac_cnt, divisors, &div_cnt);

    unsigned long long cnt[4] = {0,0,0,0};
    for (size_t i = 0; i < div_cnt; ++i)
    {
        uint64_t d = divisors[i];
        while (d > 0)
        {
            cnt[d % 4] += 1;
            d /= 4;
        }
    }
    if (cnt[0] == cnt[1] && cnt[0] == cnt[2] && cnt[0] == cnt[3] && cnt[0] > 0)
    {
        *common = cnt[0];
        return 1;
    }
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise primes once */
    init_primes();

    /* extend cache up to requested index */
    while (cache_len <= (size_t)n)
    {
        ++last_checked;
        unsigned long long common;
        if (test_number(last_checked, &common))
        {
            if (cache_len < MAX_CACHE)
                cache[cache_len++] = (seq_t)common;
        }
    }

    return cache[(size_t)n];
}
