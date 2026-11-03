/*
 * A219604  Smallest prime p such that 2n+1 = 4q + p for some odd prime q, or
 *          0 if no such prime exists.
 * https://oeis.org/A219604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that 2n+1 = 4q + p for some
 *            prime q; returns 0 if no such prime exists
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9949 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

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

static uint64_t *prime_list = NULL;   /* primes up to prime_limit */
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* 2n + 1, computed in seq_t to avoid overflow of idx_t */
    seq_t m = (seq_t)2 * (seq_t)n + 1;

    /* Build or extend the prime list to cover m (needed for both p and q) */
    if (m > (seq_t)prime_limit) {
        idx_t new_limit = (idx_t)m;
        if (prime_list != NULL) {
            free(prime_list);
            prime_list = NULL;
            prime_cnt  = 0;
        }
        build_primes_upto(new_limit, &prime_list, &prime_cnt);
        prime_limit = new_limit;
    }

    /* Search primes in increasing order for the smallest p that works */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];          /* candidate p (prime) */
        if (p > m) break;                       /* no need to check larger p */

        seq_t rem = m - p;
        if (rem & 3) continue;                  /* 4 does not divide rem */

        seq_t q = rem >> 2;                     /* q = (m - p) / 4 */

        if (is_prime_big(q))
            return p;                           /* first qualifying p is minimal */
    }

    return (seq_t)0;                            /* no representation found */
}
