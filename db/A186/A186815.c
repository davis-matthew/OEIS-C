/*
 * A186815  Numbers n such that n^2-10 is a prime.
 * https://oeis.org/A186815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 876 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the nth integer > 0 for which n^2 – 10 is prime.
    The sequence is indexed from 1 (a(1) = 9).  The routine
    generates terms as needed and remembers them in a cache
    so subsequent calls are fast.  */
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

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of terms stored */
static idx_t next_candidate = 9;   /* next integer to test */

static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();                     /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* extend cache until it contains at least `up_to` terms */
static void grow_cache(idx_t up_to)
{
    ensure_capacity(up_to);
    while (cache_len < (size_t)up_to) {
        /* find next integer whose square minus 10 is prime */
        for (;;) {
            seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate - (seq_t)10;
            if (is_prime_big(sq))
                break;
            next_candidate++;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        next_candidate++;           /* next test starts after found value */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0, but return 0 */
    if (cache_len < (size_t)n)
        grow_cache(n);
    return cache[n - 1];
}
