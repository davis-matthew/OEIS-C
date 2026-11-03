/*
 * A158725  Non-repdigit composite numbers not divisible by 2, 3, 5 or 11.
 * https://oeis.org/A158725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 9347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8956 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th element of OEIS A158725.
 * The sequence is defined as:
 *   non‑repeated “composite” numbers that are not divisible by 2, 3, 5 or 11,
 *   end in 1, 3, 7 or 9, have a digital root not a multiple of 3,
 *   and whose alternate‑digit sums differ by a number not divisible by 11.
 * A value is produced by a linear search with memoisation, so that
 * calls in arbitrary order are O(1) after the first few terms have been
 * generated. */

#include <stdbool.h>
#include <stdlib.h>
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

static bool ok_alt_sum_diff(seq_t x)
{
    seq_t sum_odd = 0, sum_even = 0;
    seq_t t = x;
    int pos = 1;                  /* position counted from the right, start 1 */
    while (t > 0) {
        seq_t d = t % 10;
        if (pos & 1)
            sum_odd += d;
        else
            sum_even += d;
        t /= 10;
        pos++;
    }
    seq_t diff = sum_odd >= sum_even ? sum_odd - sum_even : sum_even - sum_odd;
    return (diff % 11) != 0;
}

static bool ok_digit_root(seq_t x)
{
    seq_t sum = 0;
    seq_t t = x;
    while (t > 0) {
        sum += t % 10;
        t /= 10;
    }
    return (sum % 3) != 0;
}

static bool ok_divisor_filters(seq_t x)
{
    return (x % 2 != 0 && x % 3 != 0 && x % 5 != 0 && x % 11 != 0);
}

/* ---------- auxiliary predicates ----------------------------------- */
static bool ok_last_digit(seq_t x)
{
    seq_t d = x % 10;
    return (d == 1 || d == 3 || d == 7 || d == 9);
}

static bool is_repdigit(seq_t x)
{
    seq_t first = x % 10;
    seq_t t = x / 10;
    while (t > 0) {
        if (t % 10 != first)
            return false;
        t /= 10;
    }
    return true;
}

/* ---------- memoised search ------------------------------------- */
static size_t cache_len  = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* current array capacity */
static seq_t *cache       = NULL;
static idx_t last_candidate = 0;  /* last integer processed (as idx_t) */

/* Append a value to the cache, growing the array if needed. */
static void cache_append(seq_t val)
{
    if (cache_len == 0) {
        /* first allocation */
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    } else if (cache_len == cache_cap) {
        /* grow */
        cache_cap <<= 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
    cache[cache_len++] = val;
}

/* Core entry point.  `n` is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; callers will not use n==0 */

    /* Continue building the cache until it covers index n. */
    while (cache_len < n) {
        last_candidate++;                /* 1, 2, 3, …  */
        seq_t cand = (seq_t)last_candidate;

        if (!ok_last_digit(cand))           continue;
        if (!ok_divisor_filters(cand))      continue;
        if (is_repdigit(cand))              continue;
        if (!ok_digit_root(cand))           continue;
        if (!ok_alt_sum_diff(cand))         continue;
        if (is_prime_big(cand))             continue;   /* needs to be composite */

        /* Store the accepted composite. */
        cache_append(cand);
    }

    return cache[n - 1];
}
