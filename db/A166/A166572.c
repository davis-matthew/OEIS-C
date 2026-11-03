/*
 * A166572  Prime numbers containing the string 11.
 * https://oeis.org/A166572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime containing the decimal substring "11"
 * Range:     u8 n <= 3, u16 n <= 377, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 996 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* primality test for arbitrary-precision seq_t values */

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

static seq_t *cache      = NULL;   /* array of found primes          */
static size_t cache_cap  = 0;       /* current capacity of cache     */
static size_t cache_cnt  = 0;       /* number of primes stored       */
static idx_t   next_candidate = 11; /* next integer to test (idx_t)  */

/* enlarge the cache array if needed */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }
    while (cache_cap < needed) {
        cache_cap <<= 1;                           /* double it               */
        cache = mem_resize(seq_t, cache, cache_cap);
    }
}

/* test whether a decimal representation of n contains “11” */
static bool contains_11(seq_t n)
{
    seq_t prev = (seq_t)0;   /* digit sentinel, 0 cannot be *both* 1 at once */
    while (n > (seq_t)0) {
        seq_t digit = n % (seq_t)10;
        if (digit == (seq_t)1 && prev == (seq_t)1)
            return true;
        prev   = digit;
        n     /= (seq_t)10;
    }
    return false;
}

/* find the next prime ≥ next_candidate that contains “11” */
static seq_t find_next_prime_with_11(void)
{
    for (;;) {
        if (next_candidate < 2)
            next_candidate = 2;
        seq_t cand = (seq_t)next_candidate;
        if (contains_11(cand) && is_prime_big(cand)) {
            next_candidate++;      /* next search starts at cand+1 */
            return cand;
        }
        ++next_candidate;
    }
}

/* required public sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;           /* undefined for n<1, but harmless */

    /* guarantee we have at least n primes in the cache */
    if (cache_cnt < (size_t)n) {
        ensure_cache_capacity((size_t)n);
        while (cache_cnt < (size_t)n) {
            seq_t p = find_next_prime_with_11();
            cache[cache_cnt++] = p;
        }
    }
    return cache[n - 1];
}
