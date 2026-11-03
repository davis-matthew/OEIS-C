/*
 * A155938  Primes p such that 13*p + 8 is also prime.
 * https://oeis.org/A155938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 939 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime p such that 13*p+8 is also prime. */

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

static seq_t *cache_seq = NULL;   /* cached sequence terms   */
static size_t cache_len = 0;      /* number of cached terms   */
static size_t cache_cap = 0;      /* allocated capacity       */
static seq_t next_candidate = 3;  /* next odd integer to test */

/* Return the nth term of OEIS sequence A155938 (1‑based index). */
seq_t a(idx_t n)
{
    /* Subsequent indices below the offset are undefined; return 0. */
    if (n < 1) return 0;

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Search for the next qualifying prime. */
        while (1) {
            if (is_prime_big(next_candidate)) {
                seq_t q = next_candidate * (seq_t)13 + (seq_t)8;
                if (is_prime_big(q)) {
                    /* Append to cache. */
                    if (cache_len == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 4;
                        cache_seq = mem_resize(seq_t, cache_seq, cache_cap);
                    }
                    cache_seq[cache_len++] = next_candidate;
                    /* Move to the next candidate for future calls. */
                    next_candidate += (seq_t)2;
                    break;  /* Found one term, back to outer loop. */
                }
            }
            /* Not a qualifying prime – try the next odd integer. */
            next_candidate += (seq_t)2;
        }
    }

    return cache_seq[n - 1];
}
