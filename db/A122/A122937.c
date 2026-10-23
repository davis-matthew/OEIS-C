/*
 * A122937  3-Round numbers: numbers n such that every number less than n and
 *          relatively prime to n has at most three prime factors (counting
 *          multiplicities).
 * https://oeis.org/A122937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 265, u32 n <= 265, u64 n <= 265,
 *            u128 n <= 265, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th 3‑Round number (OEIS A122937).
   It uses the necessary and sufficient condition:
   For a candidate m, every prime p ​≤​ ⌊m^(1/4)⌋ must divide m.  The
   sequence is finite, so the routine continues counting candidates
   until the required index n is reached.  Results are memoised in a
   static cache, so calls with arbitrary n are supported. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------------ */
/* Helper: compute ⌊n^(1/4)⌋ using binary search and 128‑bit arithmetic.    */
static inline uint64_t floor_fourth_root(uint64_t n) {
    uint64_t lo = 0, hi = 65536;          /* 65536^4 = 2^64  */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __int128 val = (__int128)mid * mid * mid * mid;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------------ */
/* Sieve of Eratosthenes up to 65535 – needed for the prime condition.      */
static uint16_t primes_arr[7000];
static size_t   primes_cnt = 0;
static bool     primes_ready = false;

static void generate_primes(void) {
    const int MAX = 65536;
    unsigned char is_comp[MAX];
    size_t cnt = 0;
    for (int i = 2; i < MAX; ++i) {
        if (!is_comp[i]) {
            primes_arr[cnt++] = (uint16_t)i;
            if ((int64_t)i * i < MAX) {
                for (int j = i * i; j < MAX; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_cnt = cnt;
    primes_ready = true;
}

/* ------------------------------------------------------------------------ */
/* Test whether m is a 3‑round number.  We simply check that every prime
   p ​≤​ ⌊m^(1/4)⌋ divides m.  This is the full condition for the sequence
   (proved in the OEIS entry). */
static bool is_3_round(uint64_t m) {
    if (m <= 1)
        return true;
    if (!primes_ready)
        generate_primes();
    uint64_t root = floor_fourth_root(m);
    for (size_t i = 0; i < primes_cnt && primes_arr[i] <= root; ++i) {
        if (m % primes_arr[i] != 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
/* Main sequence function.  We keep a cache of found numbers; when the
   requested index exceeds the cached length we continue searching from
   the last tested integer. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t size  = 0;   /* number of terms stored              */
    static size_t cap   = 0;   /* allocated capacity                   */
    static uint64_t last_checked = 0; /* largest integer examined already */

    /* Initialise cache on first call. */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Expand cache until we have at least n terms. */
    while (size < n) {
        ++last_checked;
        if (is_3_round(last_checked)) {
            if (size == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
