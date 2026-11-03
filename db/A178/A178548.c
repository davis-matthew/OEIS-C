/*
 * A178548  a(n) is the smallest prime p such that q = 2 * p + prime(n+2) is
 *          prime AND A007953(p) = A007953(q) where A007953 is sum of digits.
 * https://oeis.org/A178548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = smallest prime p such that q = 2*p + prime(n+2) is prime
 *            and digit_sum(p) = digit_sum(q)
 * Range:     u8 n <= 2, u16 n <= 6960, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9616 (2026-11-03)
 * Helpers:   build_primes_upto, digit_sum, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"
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

static uint64_t *prime_list = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t prime_count = 0;        /* number of primes stored */
static idx_t prime_limit = 0;         /* last limit used to build primes */

/* Ensure that prime_list contains at least `need` primes (1‑based). */
static void ensure_primes_at_least(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    idx_t new_limit = prime_limit ? prime_limit * 2 : 1000;
    while (prime_count < (size_t)need) {
        uint64_t *list = NULL;
        size_t cnt = 0;
        build_primes_upto(new_limit, &list, &cnt);
        if (prime_list) free(prime_list);
        prime_list = list;
        prime_count = cnt;
        prime_limit = new_limit;
        if (prime_count >= (size_t)need)
            break;
        new_limit *= 2;
    }
}

/* Return the n‑th term of the sequence.  n is 1‑based; n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */

    /* We need the (n+2)‑th prime: index n+1 in 0‑based array. */
    ensure_primes_at_least(n + 2);
    const idx_t prime_idx_needed = n + 1;          /* 0‑based */
    const uint64_t q_prime_part = prime_list[prime_idx_needed];

    /* Iterate over primes until the condition holds. */
    for (size_t i = 0; ; ++i) {
        /* Make sure the candidate list covers i. */
        if (i >= prime_count)
            ensure_primes_at_least(i + 1);

        uint64_t p = prime_list[i];
        seq_t q = (seq_t)((seq_t)2 * p + q_prime_part);

        if (is_prime_big(q) && digit_sum(p) == digit_sum(q))
            return (seq_t)p;
    }
}
