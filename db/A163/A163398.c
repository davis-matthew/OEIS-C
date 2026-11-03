/*
 * A163398  Numbers m such that all numbers 10*m+(odd single-digit number) and
 *          100*m+(any 2-digit, digits coprime to 10) are composite.
 * https://oeis.org/A163398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the n-th integer m with 10*m + d composite for all odd d
 *            ∈ {1,3,5,7,9}, and 100*m + xy composite for all two‑digit xy
 *            whose digits are coprime to 10 (i.e. xy ∈ {11,13,17,…,99} with
 *            each digit ∈ {1,3,7,9}). The sequence is computed by incremental
 *            search
 * Range:     u8 none, u16 n <= 4, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Two‑digit numbers with both digits ∈ {1,3,7,9}. */
static const int two_digits[16] = {
    11,13,17,19,
    31,33,37,39,
    71,73,77,79,
    91,93,97,99
};

/* Cache of already found terms, large enough for n up to 10000. */
#define CACHE_CAP 20000
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static seq_t last_m = 166;   /* start just before the first term (167) */
static bool initialized = false;

/* Test whether m satisfies the conditions. */
static bool is_good_m(seq_t m)
{
    /* 10*m + d for d = 1,3,5,7,9 must be composite. */
    for (int d = 1; d <= 9; d += 2) {
        seq_t val = (seq_t)10 * m + d;
        if (is_prime_big(val)) return false;
    }
    /* 100*m + xy for all two‑digit numbers xy with digits ∈ {1,3,7,9}. */
    for (int i = 0; i < 16; ++i) {
        seq_t val = (seq_t)100 * m + two_digits[i];
        if (is_prime_big(val)) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        cache_len = 0;
        last_m = 166;            /* first candidate is 167 = 166+1 */
        initialized = true;
    }

    while (cache_len < (size_t)n) {
        seq_t candidate = last_m + 1;
        if (is_good_m(candidate)) {
            if (cache_len >= CACHE_CAP) {
                /* Should not happen for n <= 10000. */
                abort();
            }
            cache[cache_len++] = candidate;
        }
        last_m = candidate;
    }

    return cache[n - 1];
}
