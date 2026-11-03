/*
 * A062590  Variation on A029834: a discrete version of the Mangoldt function.
 *          If n is prime then floor(log(prime(n))) else 0.
 * https://oeis.org/A062590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log(prime(n))) if n is prime; otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8603 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* compute floor(log_e(n)) for n>=1 using integer comparisons only */
static int floor_log_e(uint64_t n)
{
    /* ceil(e^(k+1)) for k = 0,1,2,...
       enough up to very large primes */
    static const uint64_t ceil_e_kplus1[] = {
        3ULL,      /* ceil(e^1)  */
        8ULL,      /* ceil(e^2)  */
        21ULL,     /* ceil(e^3)  */
        55ULL,     /* ceil(e^4)  */
        149ULL,    /* ceil(e^5)  */
        404ULL,    /* ceil(e^6)  */
        1100ULL,   /* ceil(e^7)  */
        2982ULL,   /* ceil(e^8)  */
        8104ULL,   /* ceil(e^9)  */
        22027ULL,  /* ceil(e^10) */
        59875ULL,  /* ceil(e^11) */
        162755ULL, /* ceil(e^12) */
        442414ULL, /* ceil(e^13) */
        1202605ULL,/* ceil(e^14) */
        3269018ULL,/* ceil(e^15) */
        8886111ULL,/* ceil(e^16) */
        24154953ULL/* ceil(e^17) */
    };
    size_t limit = sizeof(ceil_e_kplus1) / sizeof(ceil_e_kplus1[0]);

    for (size_t k = 0; k < limit; ++k) {
        if (n < ceil_e_kplus1[k]) return (int)k;
    }
    /* For very large primes beyond the table, use an approximate
       method: floor(log_e n) ≈ floor(log2 n * ln(2)) */
    unsigned int msb = 0;
    uint64_t t = n;
    while (t > 1) {
        t >>= 1;
        ++msb;
    }
    /* ln(2) ≈ 0.6931471805599453 ≈ 749863452 / 2^35 */
    const unsigned long long LN2_NUM = 749863452ULL;
    const unsigned int LN2_DEN_EXP = 35;
    unsigned long long num = (unsigned long long)msb * LN2_NUM;
    int result = (int)(num >> LN2_DEN_EXP);
    /* verify and adjust if needed */
    while (1) {
        /* compute e^(result+1) <= n < e^(result+2) check using __int128 */
        __int128 exp_val = 1;
        for (int i = 0; i < result + 1; ++i) exp_val *= 2;
        if (exp_val > n) {
            --result;
            continue;
        }
        exp_val = 1;
        for (int i = 0; i < result + 2; ++i) exp_val *= 2;
        if (exp_val <= n) {
            ++result;
            continue;
        }
        break;
    }
    return result;
}

/* static storage for generated primes (0-indexed) */
static uint64_t *primes = NULL;
static size_t primes_alloc = 0;
static size_t primes_count = 0;
static uint64_t next_candidate = 2;

/* generate primes up to at least the k-th (1‑based) prime */
static void _ensure_primes(size_t k)
{
    if (k <= primes_count) return;

    if (!primes) {
        primes_alloc = 64;
        primes = (uint64_t *)malloc(primes_alloc * sizeof(uint64_t));
    }

    while (primes_count < k) {
        uint64_t c = next_candidate;
        bool prime_candidate = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > c) break;
            if (c % p == 0) { prime_candidate = false; break; }
        }
        while (!prime_candidate) {
            ++c;
            prime_candidate = true;
            for (size_t i = 0; i < primes_count; ++i) {
                uint64_t p = primes[i];
                if (p * p > c) break;
                if (c % p == 0) { prime_candidate = false; break; }
            }
        }
        if (primes_count >= primes_alloc) {
            primes_alloc *= 2;
            primes = (uint64_t *)realloc(primes, primes_alloc * sizeof(uint64_t));
        }
        primes[primes_count++] = c;
        next_candidate = c + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!is_prime(n)) return 0;

    _ensure_primes((size_t)n);

    uint64_t p = primes[n - 1];
    return (seq_t)floor_log_e(p);
}
