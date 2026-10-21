/*
 * A222084  Number of the least divisors of n whose LCM is equal to n.
 * https://oeis.org/A222084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_divs_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of the least divisors of n whose LCM is equal to n
   OEIS A222084.
   Implementation: tau#(n) = # { d | d divides n, d <= max(p_i^a_i) }   */

/*=== Standard includes ====================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/count_divs_le.h"

/*=== Integer square‑root for uint64_t -----------------------------------*/
static idx_t isqrt_u64(uint64_t x)
{
    idx_t r = 0, bit = (idx_t)1 << 62;   /* 2^62 <= sqrt(2^64-1) */
    while (bit) {
        idx_t nxt = r | bit;
        if ( ((__uint128_t)nxt * (__uint128_t)nxt) <= x )
            r = nxt;
        bit >>= 1;
    }
    return r;
}

/*=== Prime sieve (once) -----------------------------------------------*/
static uint64_t *prime_tbl = NULL;
static size_t   prime_cnt = 0;
static idx_t    prime_limit = 0;

static void init_primes(idx_t limit)
{
    if (limit <= prime_limit) return;          /* already prepared */

    /* free old table if any */
    if (prime_tbl) free(prime_tbl);

    /* crude estimate of number of primes up to limit */
    size_t est = (limit > 1 ? limit / 2 : 1);
    prime_tbl = (uint64_t*)malloc(est * sizeof(uint64_t));
    if (!prime_tbl) return;                     /* out of memory → abort */

    for (size_t i = 0; i < est; ++i) prime_tbl[i] = 0;

    bool *comp = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!comp) { free(prime_tbl); prime_tbl = NULL; return; }

    prime_cnt = 0;
    if (limit >= 2) {
        prime_tbl[prime_cnt++] = 2;
    }

    for (idx_t i = 3; i <= limit; i += 2) {
        if (!comp[(size_t)i]) {
            if (prime_cnt >= est) {
                est = est * 3 / 2 + 32;
                uint64_t *t = (uint64_t*)realloc(prime_tbl, est * sizeof(uint64_t));
                if (!t) break;            /* keep old array, stop */
                prime_tbl = t;
            }
            prime_tbl[prime_cnt++] = (uint64_t)i;
            if ( ((__uint128_t)i * (__uint128_t)i) <= (__uint128_t)limit ) {
                for (idx_t j = i * i; j <= limit; j += i << 1)
                    comp[(size_t)j] = true;
            }
        }
    }
    free(comp);
    prime_limit = limit;
}

/*=== Main sequence function ----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for n = 0; safe return */
    if (n == 1) return 1;       /* only divisor is 1 */

    uint64_t nn = (uint64_t)n;
    idx_t lim = isqrt_u64(nn);
    init_primes(lim);

    /* Factorisation of n */
    uint64_t pf[32];
    uint8_t  ex[32];
    size_t   pcnt = 0;

    uint64_t tmp = nn;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * (uint64_t)prime_tbl[i] <= tmp; ++i) {
        uint64_t p = prime_tbl[i];
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[pcnt] = p;
            ex[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        pf[pcnt] = tmp;
        ex[pcnt] = 1;
        ++pcnt;
    }

    /* Largest prime power in n */
    uint64_t maxP = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p_pow = 1;
        for (uint8_t k = 0; k < ex[i]; ++k) p_pow *= pf[i];
        if (p_pow > maxP) maxP = p_pow;
    }

    return count_divs_le(pf, ex, pcnt, 0, 1ULL, maxP);
}
