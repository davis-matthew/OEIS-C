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
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 52, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
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
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

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
