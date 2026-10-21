/*
 * A059911  a(n) = |{m : multiplicative order of n mod m = 6}|.
 * https://oeis.org/A059911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n^6-1)-tau(n^3-1)-tau(n^2-1)+tau(n-1)
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: standard integer types and bool */
#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Local helper data structures and constants                        */
/* ------------------------------------------------------------------ */
typedef struct { uint32_t p; uint32_t e; } Factor;

/* Prime sieve limits: sqrt(100010001) ≈ 10001 */
#define PRIME_LIMIT 10000
#define PRIME_CAP   1229      /* number of primes ≤ 10000 */

static bool primes_built = false;
static uint32_t primes[PRIME_CAP];
static size_t prime_cnt;

/* ------------------------------------------------------------------ */
/* Prime sieve: builds the list of primes ≤ PRIME_LIMIT               */
/* ------------------------------------------------------------------ */
static void build_primes(void)
{
    if (primes_built) return;

    bool sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;

    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            primes[prime_cnt++] = (uint32_t)i;
    primes_built = true;
}

/* ------------------------------------------------------------------ */
/* Factorises a 64‑bit number (≤ 1e8) into its prime factors.       */
/* The caller must provide an array of at least 32 Factor entries.   */
/* The array length is stored in *len.                               */
/* ------------------------------------------------------------------ */
static void factor_uint64(uint64_t n, Factor *fac, size_t *len)
{
    *len = 0;
    if (n <= 1) return;

    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > temp) break;
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fac[*len].p = p;
            fac[*len].e = e;
            ++(*len);
        }
    }
    if (temp > 1) {
        fac[*len].p = (uint32_t)temp;
        fac[*len].e = 1;
        ++(*len);
    }
}

/* ------------------------------------------------------------------ */
/* Merges two factor lists into one. The result is stored in *out and */
/* the final length is written to *lenout.                            */
/* ------------------------------------------------------------------ */
static void merge_factors(const Factor *a, size_t lena,
                          const Factor *b, size_t lenb,
                          Factor *out, size_t *lenout)
{
    size_t outlen = lena;
    /* copy 'a' into 'out' */
    for (size_t i = 0; i < lena; ++i) out[i] = a[i];

    /* merge each factor from 'b' into 'out' */
    for (size_t i = 0; i < lenb; ++i) {
        uint32_t p = b[i].p;
        uint32_t e = b[i].e;
        /* search for p in existing 'out' entries */
        size_t j;
        for (j = 0; j < outlen; ++j) {
            if (out[j].p == p) {
                out[j].e += e;
                break;
            }
        }
        if (j == outlen) {          /* p not found, append new factor */
            out[outlen].p = p;
            out[outlen].e = e;
            ++outlen;
        }
    }
    *lenout = outlen;
}

/* ------------------------------------------------------------------ */
/* Computes the divisor count tau from a factor list.                 */
/* ------------------------------------------------------------------ */
static seq_t tau_from_factors(const Factor *fac, size_t len)
{
    seq_t res = 1;
    for (size_t i = 0; i < len; ++i)
        res *= (seq_t)(fac[i].e + 1);
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    build_primes();

    /* Temporary factor arrays – enough space for any factorization */
    Factor f1[32], f2[32], f3[32], f4[32];
    size_t l1, l2, l3, l4;

    /* 1. Factor n-1 */
    factor_uint64((uint64_t)n - 1, f1, &l1);
    seq_t tau1 = tau_from_factors(f1, l1);

    /* 2. Factor n+1 */
    factor_uint64((uint64_t)n + 1, f2, &l2);

    /* 3. Factor n^2 + n + 1 */
    uint64_t val3 = (uint64_t)n * (uint64_t)n + (uint64_t)n + 1;
    factor_uint64(val3, f3, &l3);

    /* 4. Factor n^2 - n + 1 */
    uint64_t val4 = (uint64_t)n * (uint64_t)n - (uint64_t)n + 1;
    factor_uint64(val4, f4, &l4);

    /* Compute tau(n^6-1) by merging all four factor lists */
    Factor merged6[64];
    size_t l6_12, l6_123, l6_all;
    merge_factors(f1, l1, f2, l2, merged6, &l6_12);
    merge_factors(merged6, l6_12, f3, l3, merged6, &l6_123);
    merge_factors(merged6, l6_123, f4, l4, merged6, &l6_all);
    seq_t tau6 = tau_from_factors(merged6, l6_all);

    /* Compute tau(n^3-1) = tau((n-1)(n^2+n+1)) */
    Factor merged3[64];
    size_t l3_merged;
    merge_factors(f1, l1, f3, l3, merged3, &l3_merged);
    seq_t tau3 = tau_from_factors(merged3, l3_merged);

    /* Compute tau(n^2-1) = tau((n-1)(n+1)) */
    Factor merged2[64];
    size_t l2_merged;
    merge_factors(f1, l1, f2, l2, merged2, &l2_merged);
    seq_t tau2 = tau_from_factors(merged2, l2_merged);

    /* Final formula */
    seq_t result = tau6 - tau3 - tau2 + tau1;
    return result;
}
