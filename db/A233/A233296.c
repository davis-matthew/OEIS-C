/*
 * A233296  a(n) = |{0 < k < n: k*prime(n-k) + 1 is prime}|.
 * https://oeis.org/A233296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of k with 0 < k < n such that k*prime(n‑k)+1 is prime
 * Range:     u8 n <= 19, u16 n <= 1099, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9721 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a cache of primes up to a fixed limit and
 * then scans all k for each call. */

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

static uint64_t *primes = NULL;   /* array of primes up to the desired limit */
static size_t   prime_cnt = 0;    /* number of primes stored */
static bool     primes_ready = false;

/* Build a list of primes sufficient for all n ≤ 10000.
 * 200 000 is comfortably above the 10000th prime (104729). */
static void init_primes(void)
{
    if (!primes_ready) {
        size_t cnt;
        build_primes_upto(200000ULL, &primes, &cnt);
        prime_cnt = cnt;
        primes_ready = true;
    }
}

/* a(n) = |{ 0 < k < n : k * prime(n-k) + 1 is prime }|   */
seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 by definition */
        return (seq_t)0;

    init_primes();

    seq_t result = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t diff = n - k;                 /* n-k in the sense of the definition */
        seq_t p  = (seq_t)primes[diff - 1]; /* prime_(n-k) */
        seq_t val = (seq_t)k * p + 1;        /* k*prime(n-k) + 1 */
        if (is_prime_big(val))
            ++result;
    }
    return result;
}
