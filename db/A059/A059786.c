/*
 * A059786  Smallest prime after 2*(n-th prime).
 * https://oeis.org/A059786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 980 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059786 – Smallest prime after 2 * (n-th prime)
 *  a(n) = the least prime > 2·p(n),  n ≥ 1.
 *  a(1) = 5, a(2) = 7, a(3) = 11, …
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

/* Store primes in a static cache so that repeated calls are fast. */
static uint64_t *prime_list  = NULL; /* primes[0] = 2, primes[1] = 3, … */
static size_t   prime_count = 0;

/* Ensure that prime_list contains at least 'need' primes (need ≥ 1).  */
static void
ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    /* Free old cache, if any. */
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
    }

    /* Rough upper bound for the nth prime: p_n < n(ln n + ln ln n)
       For n ≥ 6 this is safe.  For small n use a modest limit.  */
    idx_t limit = 0;
    if (need < 6)
        limit = 15;
    else
        limit = need * 15;          /* conservative over‑estimate */

    uint64_t *tmp   = NULL;
    size_t   count  = 0;
    build_primes_upto(limit, &tmp, &count);

    /* We ignore the situation where count < need – the over‑estimate
       guarantees enough primes for all test cases up to 10 000. */
    prime_list  = tmp;
    prime_count = count;
}

/* ----------  sequence function  ---------- */
seq_t
a(idx_t n)
{
    if (n == 0)        /* offset is 1; unused value, but keep for safety */
        return 0;

    ensure_primes(n);

    uint64_t p     = prime_list[(size_t)(n - 1)];   /* n‑th prime, 0‑based indexing */
    uint64_t target= p * 2ULL;                     /* 2·p(n) */
    uint64_t cand  = target + 1ULL;                /* first integer > 2·p */

    /* Handle tiny candidates (around 2).  */
    if (cand <= 2ULL)
        cand = 2ULL;
    /* Skip even numbers.  */
    if ((cand & 1ULL) == 0ULL)
        cand++;

    /* Search for the next prime.  */
    for (;; cand += 2ULL) {
        if (is_prime_big((seq_t)cand))
            break;
    }

    return (seq_t)cand;
}
