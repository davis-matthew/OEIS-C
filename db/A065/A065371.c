/*
 * A065371  a(1) = 1, a(prime(i)) = prime(i) - i for i > 0 and a(u * v) = a(u)
 *          * a(v) for u, v > 0.
 * https://oeis.org/A065371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = 1 if n = 1 a(p_k) = p_k – k for p_k prime, k ≥ 1 a(uv) =
 *            a(u) * a(v) for u, v > 0, multiplicative (Indices are 1‑based:
 *            p_1 = 2, p_2 = 3, …)
 * Range:     u8 n <= 330, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------- */
/*  Prime table (sieve of Eratosthenes) on demand             */
/* ----------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* static prime storage */
static uint32_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_lim   = 0;   /* last element in prime_list is ≤ prime_lim */

/* build primes up to at least 'lim' (inclusive) */
static void ensure_primes(size_t lim)
{
    if (prime_lim >= lim) return;

    /* allocate sieve array; cast required for C++ */
    bool *sieve = (bool *)malloc((lim + 1) * sizeof(bool));
    if (!sieve) abort();
    memset(sieve, true, (lim + 1) * sizeof(bool));
    if (lim >= 0) sieve[0] = false;
    if (lim >= 1) sieve[1] = false;

    for (size_t i = 2; i * i <= lim; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= lim; j += i)
                sieve[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (sieve[i]) ++cnt;

    /* reallocate prime_list */
    uint32_t *new_list = (uint32_t *)realloc(prime_list, cnt * sizeof(uint32_t));
    if (!new_list) abort();
    prime_list = new_list;
    prime_cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = (uint32_t)i;

    prime_lim = lim;
    free(sieve);
}

/* binary search: 1‑based index of prime 'p'  (p is guaranteed to be prime) */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint64_t)prime_list[mid] >= p)
            hi = mid;
        else
            lo = mid + 1;
    }
    if (lo < prime_cnt && (uint64_t)prime_list[lo] == p)
        return lo + 1;        /* 1‑based */
    return 0;                /* should not happen */
}

/* ----------------------------------------------------------- */
/*  Main function required by the interface                   */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Ensure we know all primes up to n, needed for prime indices. */
    ensure_primes((size_t)n);

    uint64_t m = (uint64_t)n;

    /* factorization: store distinct primes and exponents */
    uint64_t fac[64];
    unsigned exp[64];
    size_t fac_cnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            fac[fac_cnt] = p;
            exp[fac_cnt] = e;
            fac_cnt++;
        }
    }
    if (m > 1) {                     /* remaining prime factor > sqrt(n) */
        fac[fac_cnt] = m;
        exp[fac_cnt] = 1;
        fac_cnt++;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p   = fac[i];
        size_t   idx = prime_index(p);          /* 1‑based prime index */
        seq_t   base = (seq_t)p - (seq_t)idx;   /* p - k   */
        seq_t   power = (seq_t)1;
        for (unsigned k = 0; k < exp[i]; ++k)
            power *= base;                      /* power = (p-k)^e   */
        result *= power;
    }
    return result;
}
