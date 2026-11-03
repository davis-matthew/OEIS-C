/*
 * A219052  Number of ways to write n = p + q(3 - (-1)^n)/2 with q <= n/2 and
 *          p, q, p^2 + q^2 - 1 all prime.
 * https://oeis.org/A219052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of primes q ≤ n/2 such that p = n - q*(3-(-1)^n)/2
 *            is prime and p^2 + q^2 - 1 is prime
 * Range:     u8 n <= 19, u16 n <= 1101, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 9553 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Store the prime list and its current maximum */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static uint64_t  prime_max   = 0;

/* Ensure we have primes up to at least 'limit'. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_list != NULL && limit <= prime_max) return;
    uint64_t *new_list = NULL;
    size_t   new_cnt   = 0;
    build_primes_upto(limit, &new_list, &new_cnt);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_max  = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* factor = 1 if n even, 2 if n odd */
    uint64_t factor = (n % 2 == 0) ? 1 : 2;
    uint64_t limit_q = n / 2;                  /* q ≤ n/2 */

    ensure_primes_up_to(limit_q);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit_q; ++i) {
        uint64_t q = prime_list[i];
        uint64_t p = n - factor * q;
        if (p == 0) continue;                  /* p must be positive */
        if (!is_prime_big((seq_t)p)) continue; /* p must be prime */

        seq_t val = (seq_t)p * (seq_t)p
                  + (seq_t)q * (seq_t)q
                  - (seq_t)1;
        if (is_prime_big(val)) count++;
    }
    return count;
}
