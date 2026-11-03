/*
 * A171581  The natural numbers without primes > 3.
 * https://oeis.org/A171581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: natural numbers that are not prime or are prime <= 3
   (i.e. 1, 2, 3, and all composite numbers). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Grow a dynamically allocated array of seq_t values.  Works in C and C++. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}

/* Cache to store the sequence values. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t  next_candidate;   /* next integer to examine */

/* Initialise the cache and the first candidate value. */
static void init_cache(void)
{
    if (cache) return;            /* already initialised */
    cache        = NULL;
    cache_len    = 0;
    cache_cap    = 0;
    next_candidate = (seq_t)1;     /* start from 1 */
}

/* Ensure that at least N terms are present in the cache. */
static void ensure_cache(size_t N)
{
    if (cache_len >= N) return;

    init_cache();
    grow_seq(&cache, &cache_cap, N);

    while (cache_len < N)
    {
        /* Skip primes greater than 3. */
        if (next_candidate > (seq_t)3 && is_prime_big(next_candidate))
        {
            next_candidate = next_candidate + (seq_t)1;
            continue;
        }

        /* Accept the current candidate. */
        cache[cache_len++] = next_candidate;
        next_candidate = next_candidate + (seq_t)1;
    }
}

/* The required OEIS function. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    size_t N = (size_t)n;   /* for array indexing */
    ensure_cache(N);
    return cache[N - 1];
}
