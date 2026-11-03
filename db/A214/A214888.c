/*
 * A214888  Primes congruent to {2, 3} mod 11.
 * https://oeis.org/A214888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   n‑th prime that is ≡ 2 or ≡ 3 (mod 11)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 996 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for A214888 – primes congruent to 2 or 3 modulo 11.
   The function keeps a static cache of the primes seen so far so that
   repeated calls are fast.  The cache grows on demand.
   The implementation works for any seq_t provided by the build
   (uint8_t …, unsigned __int128, or arbitrary‑precision integer classes). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static cache of primes ≡ 2 or 3 (mod 11) */
static seq_t *prime_cache     = NULL;
static size_t prime_cache_cap  = 0;
static size_t prime_cache_len  = 0;
static idx_t  next_candidate   = 2;   /* start from the first integer */

seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined offset – return 0 */
        return (seq_t)0;

    /* make sure the cache holds at least n primes */
    while (prime_cache_len < n) {
        /* search for the next qualifying prime */
        for ( ; ; ++next_candidate) {
            /* quick skip numbers not ≡ 2 or 3 (mod 11) */
            uint64_t r = next_candidate % 11;
            if (r != 2 && r != 3)
                continue;

            /* primality test for arbitrary precision */
            if (is_prime_big((seq_t)next_candidate)) {
                /* grow cache if necessary */
                grow_seq(&prime_cache, &prime_cache_cap,
                         prime_cache_len + 1);
                /* store the prime */
                prime_cache[prime_cache_len++] = (seq_t)next_candidate;
                /* advance to the next candidate for future searches */
                ++next_candidate;
                break;                    /* found next term */
            }
        }
    }

    return prime_cache[n - 1];
}
