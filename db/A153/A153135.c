/*
 * A153135  Primes p such that 6*p - 7 is also prime.
 * https://oeis.org/A153135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 447, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 965 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = prime p such that 6*p – 7 is also prime. */

/* The routine builds the sequence on demand, caching results for
   subsequent calls.  It tests each integer candidate for primality
   with the provided is_prime_big routine and accepts the prime if
   the companion value 6*p‑7 is also prime.  The cache grows as
   necessary, so the function works for any order of calls and for
   large n.  All arithmetic is performed in seq_t; the candidate
   counter is held in seq_t and advanced only by the necessary
   increments. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset: queries start at 1 */

    /* ---------- Cache of computed terms ---------- */
    static seq_t *cache = NULL;          /* array of accepted primes   */
    static size_t cache_cap = 0;          /* allocated size               */
    static size_t cache_len = 0;          /* number of terms stored      */

    /* ---------- Candidate counter for prime search ---------- */
    static seq_t candidate = 0;           /* next integer to test       */
    static bool first_call = true;        /* initialise on first use    */

    /* ---------- Initialisation of candidate ---------- */
    if (first_call) {
        candidate = 2;                     /* start with the first prime */
        first_call = false;
    }

    /* ---------- Extend cache capacity if needed ---------- */
    if (cache_len < (size_t)n && cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;                /* allocation failure          */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---------- Generate terms until we reach n ---------- */
    while (cache_len < (size_t)n) {
        /* Skip non‑prime candidates fast: after 2 only odds */
        if (candidate > 2 && (candidate & 1) == 0) {
            candidate += 1;
            continue;
        }

        /* Test primality of candidate */
        if (is_prime_big(candidate)) {
            /* Check companion number 6*p – 7 */
            seq_t companion = candidate * 6 - 7;
            if (is_prime_big(companion)) {
                cache[cache_len++] = candidate;
            }
        }

        /* Advance candidate */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;                  /* jump to next odd */
    }

    return cache[n - 1];
}
