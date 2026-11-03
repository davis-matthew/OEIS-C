/*
 * A079016  Suppose p and q = p+12 are primes. Define the difference pattern
 *          of (p,q) to be the successive differences of the primes in the
 *          range p to q. There are 14 possible difference patterns, namely
 *          [12], [2,10], [4,8], [6,6], [8,4], [10,2], [2,4,6], [2,6,4],
 *          [4,2,6], [4,6,2], [6,2,4], [6,4,2], [2,4,2,4...
 * https://oeis.org/A079016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: full, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079016: for each of the 14 fixed prime‑difference patterns occurring
 * between primes p and p+12 (with p and p+12 prime), return the smallest such
 * p. The sequence consists of these 14 minimal primes, sorted increasingly.
 * The values are computed by searching primes until all patterns are found.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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

/* Minimal prime for each pattern, sorted after initialization. */
static seq_t min_p[14] = {0};
static bool initialized = false;

/* The 14 patterns and their lengths. */
static const struct {
    int len;
    int diff[4];
} patterns[14] = {
    {1, {12, 0, 0, 0}},          /* [12]           */
    {2, {2, 10, 0, 0}},          /* [2,10]         */
    {2, {4, 8, 0, 0}},           /* [4,8]          */
    {2, {6, 6, 0, 0}},           /* [6,6]          */
    {2, {8, 4, 0, 0}},           /* [8,4]          */
    {2, {10, 2, 0, 0}},          /* [10,2]         */
    {3, {2, 4, 6, 0}},           /* [2,4,6]        */
    {3, {2, 6, 4, 0}},           /* [2,6,4]        */
    {3, {4, 2, 6, 0}},           /* [4,2,6]        */
    {3, {4, 6, 2, 0}},           /* [4,6,2]        */
    {3, {6, 2, 4, 0}},           /* [6,2,4]        */
    {3, {6, 4, 2, 0}},           /* [6,4,2]        */
    {4, {2, 4, 2, 4}},           /* [2,4,2,4]      */
    {4, {4, 2, 4, 2}}            /* [4,2,4,2]      */
};

/* Return the index of the pattern matching the given difference array,
 * or -1 if no pattern matches. */
static int match_pattern(const int *diffs, int len)
{
    for (int i = 0; i < 14; ++i) {
        if (patterns[i].len != len)
            continue;
        bool ok = true;
        for (int j = 0; j < len; ++j) {
            if (diffs[j] != patterns[i].diff[j]) {
                ok = false;
                break;
            }
        }
        if (ok)
            return i;
    }
    return -1;
}

/* Initialise the minimal prime table once. */
static void init(void)
{
    if (initialized)
        return;

    size_t found = 0;
    uint64_t p = 1;                 /* start before the first prime */

    while (found < 14) {
        p = next_prime(p);           /* next prime after p */
        uint64_t q = p + 12;
        if (!is_prime_big((seq_t)q))
            continue;

        /* Build the difference pattern between p and q. */
        int diffs[4] = {0};
        int cnt = 0;
        uint64_t prev = p;
        uint64_t nxt  = next_prime(prev);
        while (nxt <= q) {
            diffs[cnt++] = (int)(nxt - prev);
            prev = nxt;
            nxt  = next_prime(prev);
        }

        int idx = match_pattern(diffs, cnt);
        if (idx < 0)
            continue;                 /* not one of the 14 patterns */

        if (min_p[idx] == 0) {         /* first occurrence of this pattern */
            min_p[idx] = (seq_t)p;
            ++found;
        }
    }

    /* Sort the found primes increasingly. */
    for (size_t i = 0; i < 14; ++i) {
        for (size_t j = i + 1; j < 14; ++j) {
            if (min_p[j] < min_p[i]) {
                seq_t tmp = min_p[i];
                min_p[i] = min_p[j];
                min_p[j] = tmp;
            }
        }
    }

    initialized = true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init();
    if (n == 0 || n > 14)
        return 0;                  /* undefined outside the 14 terms */
    return min_p[(size_t)n - 1];
}
