/*
 * A075241  Least base for which the n-th prime has no prime reversal in that
 *          base.
 * https://oeis.org/A075241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the least base b such that the n-th prime has no prime *
 *            reversal in base b
 * Range:     u8 n <= 43, u16 n <= 7257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 8923 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache for primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    /* grow the primes table if needed */
    if (n > primes_cnt) {
        /* compute a sufficient limit */
        idx_t limit = _prime_limit_for_idx(n);
        if (limit > primes_limit) {
            uint64_t *new_primes = NULL;
            size_t new_cnt = 0;
            build_primes_upto(limit, &new_primes, &new_cnt);
            free(primes);
            primes = new_primes;
            primes_cnt = new_cnt;
            primes_limit = limit;
        }
    }

    /* the n-th prime (1‑based index) */
    uint64_t P = primes[n - 1];

    /* try each base from 2 upward until a base with a composite reversal is found */
    for (idx_t base = 2; base <= P; ++base) {
        /* collect the digits of P in base 'base' */
        uint32_t digits[64];
        size_t len = 0;
        uint64_t tmp = P;
        if (tmp == 0) digits[len++] = 0;  /* keep for P==0, not used here */
        while (tmp > 0) {
            digits[len++] = (uint32_t)(tmp % (uint64_t)base);
            tmp /= (uint64_t)base;
        }

        /* compute the reversed value */
        seq_t rev = 0;
        for (size_t i = 0; i < len; ++i) {
            rev = rev * (seq_t)base + (seq_t)digits[i];
        }

        /* 1 or 0 is not prime, otherwise check primality */
        if (rev < 2) return (seq_t)base;
        if (!is_prime_big(rev)) return (seq_t)base;
    }

    /* The loop always succeeds because base == prime itself yields rev=1. */
    return (seq_t)P;
}
