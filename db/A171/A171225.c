/*
 * A171225  Nonnegative numbers which cannot be represented by inserting plus
 *          or minus signs between the digits of 123456789.
 * https://oeis.org/A171225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = smallest nonnegative integer that cannot be represented
 *            by inserting plus or minus signs (or concatenation) between the
 *            digits of 123456789, offset 1. The function computes this by
 *            enumerating all 3^8 expressions, collecting all representable
 *            non‑negative values, and then generating non‑re...
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TOTAL_COMB 6561U /* 3^8 */

static seq_t repVals[TOTAL_COMB];
static size_t repCount = 0;

static void init_representables(void)
{
    size_t i, cnt = 0;
    for (size_t mask = 0; mask < TOTAL_COMB; ++mask) {
        long long current = 1;   /* first digit */
        long long sum = 0;
        uint64_t tmp = mask;
        for (int pos = 0; pos < 8; ++pos) {
            int action = tmp % 3;          /* 0: concat, 1: '+', 2: '-' */
            tmp /= 3;
            int digit = pos + 2;           /* digits 2..9 */
            long long dig = digit;
            if (action == 0) {             /* concatenation */
                if (current >= 0)
                    current = current * 10 + dig;
                else
                    current = current * 10 - dig;
            } else if (action == 1) {      /* plus */
                sum += current;
                current = dig;
            } else {                       /* minus */
                sum += current;
                current = -dig;
            }
        }
        sum += current;
        if (sum >= 0) {
            repVals[cnt++] = (seq_t)sum;
        }
    }
    /* Sort the array (bubble sort – works for the small size). */
    for (i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (repVals[j] < repVals[i]) {
                seq_t tmpVal = repVals[i];
                repVals[i] = repVals[j];
                repVals[j] = tmpVal;
            }
        }
    }
    /* Remove duplicates. */
    size_t uniq = 0;
    for (i = 0; i < cnt; ++i) {
        if (i == 0 || repVals[i] != repVals[i - 1]) {
            repVals[uniq++] = repVals[i];
        }
    }
    repCount = uniq;
}

static bool is_represented(seq_t x)
{
    size_t lo = 0, hi = repCount;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (repVals[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < repCount && repVals[lo] == x);
}

/* Cache of non‑representable numbers. */
static seq_t *cache = NULL;
static size_t cacheSize = 0;
static size_t cacheCap = 0;
static unsigned long long nextCandidate = 0; /* next non‑negative integer to test */

static void ensure_cache_capacity(size_t needed)
{
    if (cacheCap < needed) {
        size_t newCap = cacheCap ? cacheCap << 1 : 64;
        while (newCap < needed) newCap <<= 1;
#ifdef __cplusplus
        seq_t *newArr = new seq_t[newCap];
#else
        seq_t *newArr = (seq_t *)malloc(newCap * sizeof(seq_t));
#endif
        if (!newArr) exit(1);
        if (cache) {
            memcpy(newArr, cache, cacheSize * sizeof(seq_t));
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }
        cache = newArr;
        cacheCap = newCap;
    }
}

static void add_cache(seq_t val)
{
    ensure_cache_capacity(cacheSize + 1);
    cache[cacheSize++] = val;
}

seq_t a(idx_t n)
{
    static bool reps_initialized = false;
    if (!reps_initialized) {
        init_representables();
        reps_initialized = true;
    }

    while (cacheSize < (size_t)n) {
        seq_t cand = (seq_t)nextCandidate;
        if (!is_represented(cand))
            add_cache(cand);
        ++nextCandidate;
    }

    return cache[n - 1];
}
