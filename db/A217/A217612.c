/*
 * A217612  Difference between n-th prime and the smallest semiprime greater
 *          than it.
 * https://oeis.org/A217612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A102414(n) - A000040(n) Where A102414(n) is the smallest
 *            semiprime > the n‑th prime
 * Range:     u8 n <= 1229, u16 n <= 1229, u32 n <= 1229, u64 n <= 1229,
 *            u128 n <= 1229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------
   This implementation pre‑computes primes up to the n‑th prime
   using trial division, and pre‑computes an array of smallest
   prime factors (SPF) up to that prime plus a margin.  For each
   requested prime it scans upward until it finds a semiprime.
   ------------------------------------------------------------------ */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t primes[20000];        /* enough for n up to at least 10000 */
static size_t primes_cnt = 0;
static uint64_t next_candidate = 2;   /* next number to test for primality */

/* ----- Prime table ----------------------------------------------- */
static bool is_prime_trial(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

static uint64_t nth_prime(idx_t n)
{
    while (primes_cnt < n)
    {
        if (is_prime_trial(next_candidate))
            primes[primes_cnt++] = next_candidate;
        ++next_candidate;
    }
    return primes[n - 1];
}

/* ----- Smallest prime factor table (SPF) ------------------------- */
static uint64_t *spf = NULL;
static size_t spf_len = 0;

static void ensure_spf_limit(idx_t lim)
{
    size_t need = (size_t)lim + 1;
    if (spf && spf_len > lim) return;

    /* Reallocate (or allocate first time) */
    free(spf);
    spf = (uint64_t *)malloc(need * sizeof(uint64_t));
    if (!spf) abort();              /* abort on bad allocation */
    spf_len = need;

    /* Sieve for SPF */
    for (size_t i = 0; i < spf_len; ++i) spf[i] = 0;
    if (spf_len > 0) spf[0] = 0;
    if (spf_len > 1) spf[1] = 1;

    for (size_t i = 2; i < spf_len; ++i)
    {
        if (!spf[i])
        {
            spf[i] = i;
            if (i * i < spf_len)
                for (size_t j = i * i; j < spf_len; j += i)
                    if (!spf[j]) spf[j] = i;
        }
    }
}

/* ----- Test if a number is a semiprime --------------------------- */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;
    int count = 0;
    while (n > 1 && count <= 2)
    {
        uint64_t p = spf[(size_t)n];
        if (p == 0) p = n;
        while (n % p == 0)
        {
            n /= p;
            ++count;
            if (count > 2) return false;
        }
    }
    return (count == 2 && n == 1);
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* defined for n >= 1 only */

    uint64_t prime = nth_prime(n);

    /* Ensure SPF table covers the prime plus a small safety margin */
    idx_t need_limit = prime + 1000;
    ensure_spf_limit(need_limit);

    uint64_t candidate = prime + 1;
    while (!is_semiprime(candidate))
        ++candidate;

    seq_t diff = (seq_t)(candidate - prime);
    return diff;
}
