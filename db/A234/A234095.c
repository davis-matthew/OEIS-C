/*
 * A234095  Primes p such that 2*p + 1 is semiprime.
 * https://oeis.org/A234095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 934 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234095: primes p such that 2*p+1 is a product of two distinct primes. */
/* In OEIS notation: a(n) = the nth prime p with 2p+1 semiprime (distinct factors). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"
#include "../utils/is_squarefree.h"
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

#define CACHE_CAP 10001   /* enough for n up to 10000; safety buffer */

static seq_t cache[CACHE_CAP];   /* stored sequence values */
static size_t cache_len = 0;     /* number of values currently stored */
static seq_t candidate = 3;      /* next odd number to test (prime or not) */

/* Test whether n is a product of two distinct primes. */
static bool is_semiprime_distinct(seq_t n)
{
    if (n <= 1)
        return false;          /* n must be >1 */
    if (!is_squarefree(n))
        return false;          /* must be square‑free to have distinct factors */
    return bigomega_seq(n) == (seq_t)2;
}

/* Append a new value to the cache. */
static void cache_append(seq_t val)
{
    if (cache_len < CACHE_CAP)
        cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1; safe fallback */

    /* Ensure we have computed at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find next prime candidate. */
        while (!is_prime_big(candidate))
            candidate += (seq_t)2;

        seq_t two_p_plus_one = candidate * (seq_t)2 + (seq_t)1;

        if (is_semiprime_distinct(two_p_plus_one))
            cache_append(candidate);

        candidate += (seq_t)2;   /* advance to the next odd number */
    }

    return cache[n - 1];
}
