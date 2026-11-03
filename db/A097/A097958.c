/*
 * A097958  Primes p such that p divides 6^((p-1)/2) - 3^((p-1)/2).
 * https://oeis.org/A097958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3264, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9342 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097958: primes p such that p | 6^((p-1)/2) – 3^((p-1)/2)
   (equivalently, p = 3 or p ≡ 1 or 7 (mod 8)) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t *primes = NULL;      /* cached list of terms */
static size_t   primes_cnt = 0;   /* number of cached terms   */
static size_t   primes_cap = 0;   /* allocated capacity       */
static seq_t    next_candidate = (seq_t)5;  /* next odd integer to test */

/* Ensure that at least n terms of the sequence are cached. */
static void ensure_primes(idx_t n)
{
    /* Initialise the cache if it is empty.  The first term is 3. */
    if (primes_cnt == 0) {
        primes_cap = 16;
        primes = mem_new(seq_t, primes_cap);
        if (!primes) abort();          /* allocation failure */
        primes[0] = (seq_t)3;
        primes_cnt = 1;
        next_candidate = (seq_t)5;
    }

    while (primes_cnt < n) {
        /* Search the next qualifying prime. */
        seq_t p = next_candidate;
        for (;;) {
            if (is_prime_big(p)) {
                /* p ≡ 1 or 7 (mod 8) */
                if (((p & (seq_t)7) == (seq_t)1) || ((p & (seq_t)7) == (seq_t)7)) {
                    /* Grow the cache if necessary. */
                    if (primes_cnt == primes_cap) {
                        size_t new_cap = primes_cap * 2;
                        seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                        if (!tmp) abort();       /* allocation failure */
                        primes = tmp;
                        primes_cap = new_cap;
                    }
                    primes[primes_cnt++] = p;
                    next_candidate = p + (seq_t)2;   /* skip to the next odd number */
                    break;                          /* proceed to the next required term */
                }
            }
            p += (seq_t)2;  /* keep only odd numbers */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 for safety */
    ensure_primes(n);
    return primes[n - 1];
}
