/*
 * A097376  Numbers n such that there is exactly one prime in a sequence
 *          formed from the t digits of n where the latter terms are given by
 *          rule b(i)=sum of t previous terms and the primes are counted from
 *          initial t digits up to the largest term < n^2; where ones are
 *          located in A098598.
 * https://oeis.org/A097376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A097376: numbers n such that the sequence beginning with the decimal
 * digits of n and extended by summing the previous t digits (t = number
 * of digits of n) contains exactly one prime among all generated terms
 * that are strictly less than n^2.
 * The function a(idx_t n) returns the n-th such number.
 */

#include <stdbool.h>
#include <stddef.h>
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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

#define MAX_T 20

static seq_t         *cache      = NULL;
static size_t        cache_cap  = 0;   /* allocated capacity */
static size_t        cache_size = 0;   /* number of stored terms */
static idx_t         last_checked = 0; /* last integer that was tested */

/* Count the primes in the sequence generated from val */
static bool is_good_number(idx_t val)
{
    /* t = number of decimal digits of val */
    int t = 0;
    idx_t temp = val;
    do {
        ++t;
        temp /= 10;
    } while (temp > 0);

    if (t == 1)   /* for single‑digit numbers the sequence never exceeds val^2 */
        return false;

    /* Extract digits from most significant to least significant */
    uint8_t digits[MAX_T];
    temp = val;
    for (int i = t - 1; i >= 0; --i) {
        digits[i] = (uint8_t)(temp % 10);
        temp /= 10;
    }

    /* Prepare first t terms */
    seq_t prev[MAX_T];
    seq_t sum = 0;
    int prime_cnt = 0;
    for (int i = 0; i < t; ++i) {
        prev[i] = (seq_t)digits[i];
        sum += prev[i];
        if (is_prime_big(prev[i]))
            ++prime_cnt;
    }

    seq_t n_sq = ((seq_t)val * (seq_t)val);

    while (1) {
        seq_t next = 0;
        for (int i = 0; i < t; ++i)
            next += prev[i];

        if (next >= n_sq)
            break;

        if (is_prime_big(next))
            ++prime_cnt;

        /* update the sliding sum and shift prev left */
        sum += next;
        sum -= prev[0];
        for (int i = 0; i < t - 1; ++i)
            prev[i] = prev[i + 1];
        prev[t - 1] = next;
    }

    return prime_cnt == 1;
}

/* Ensure capacity of the cache array */
static void ensure_cache_capacity(size_t needed)
{
    if (needed > cache_cap)
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0, but guard against accidental use */
        return 0;

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size)
        return cache[(size_t)(n - 1)];

    /* Extend cache until we have the desired term */
    while ((size_t)n > cache_size) {
        ++last_checked;
        if (is_good_number(last_checked)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size] = (seq_t)last_checked;
            ++cache_size;
        }
    }
    return cache[(size_t)(n - 1)];
}
