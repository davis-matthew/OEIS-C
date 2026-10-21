/*
 * A210444  a(n) = |{0<k<n: k*n is practical, k*n-1 and k*n+1 are twin
 *          primes}|.
 * https://oeis.org/A210444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {0 < k < n : k*n is practical and k*n-1 and k*n+1 are
 *            twin primes}
 * Range:     u8 n <= 2729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Simple prime sieve (up to 200 000) used for all factorisations    */
/* ------------------------------------------------------------------ */
#define MAX_PRIME_LIST 20000
#define PRIME_SIEVE_LIMIT 200000

static bool primes_init = false;
static uint64_t prime_list[MAX_PRIME_LIST];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_init) return;
    bool *is_comp = (bool *)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_comp) return;    /* out of memory – unlikely for this limit */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (idx < MAX_PRIME_LIST) prime_list[idx++] = i;
            if (i * i <= PRIME_SIEVE_LIMIT) {
                for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    prime_cnt = idx;
    free(is_comp);
    primes_init = true;
}

/* ------------------------------------------------------------------ */
/*  Factorisation: fills arrays 'factors' & 'exps', returns #factors   */
/* ------------------------------------------------------------------ */
static size_t factor_uint64(uint64_t n, uint64_t factors[], int exps[])
{
    size_t count = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) {
            int e = 0;
            while (n % prime_list[i] == 0) {
                n /= prime_list[i];
                ++e;
            }
            factors[count] = prime_list[i];
            exps[count] = e;
            ++count;
        }
    }
    if (n > 1) {
        /* remaining prime factor */
        factors[count] = n;
        exps[count] = 1;
        ++count;
    }
    return count;
}

/* ------------------------------------------------------------------ */
/*  Practical‑number test                                               */
/* ------------------------------------------------------------------ */
static bool is_practical(uint64_t m)
{
    if (m <= 1) return true;   /* 1 is practical */

    uint64_t fac[MAX_PRIME_LIST];
    int exp[MAX_PRIME_LIST];
    size_t cnt = factor_uint64(m, fac, exp);
    if (cnt == 0) return true; /* m was 1 already handled */

    if (fac[0] != 2) return false;

    /* sigma of 2^exp[0] */
    __int128 sigma = (( (__int128)1 << (exp[0] + 1) ) - 1);

    for (size_t i = 1; i < cnt; ++i) {
        uint64_t p = fac[i];
        if (p > (uint64_t)(sigma + 1)) return false;

        /* sigma of p^exp[i] */
        __int128 powp = 1;
        for (int e = 0; e < exp[i] + 1; ++e) powp *= p;
        __int128 sigma_i = (powp - 1) / (p - 1);

        sigma *= sigma_i;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Primality test (trial division using the precomputed primes)      */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();          /* ensure prime list ready */
    seq_t cnt = 0;

    /* iterate k = 1 .. n-1 (unsigned 64‑bit counters) */
    for (idx_t k = 1; k < n; ++k) {
        uint64_t m = (uint64_t)k * (uint64_t)n;
        if (!is_prime_uint64(m - 1)) continue;
        if (!is_prime_uint64(m + 1)) continue;
        if (is_practical(m)) ++cnt;
    }
    return cnt;
}
