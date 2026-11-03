/*
 * A044904  Numbers whose base-3 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 991, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 948 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑3 run lengths alternate odd, even, odd, … (first run odd). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Check whether x satisfies the alternating‑parity rule in base 3. */
static bool passes_base3_parity(seq_t x)
{
    if (x == 0)
        return false;

    /* Store base‑3 digits (least significant first). */
    int *digits = NULL;
    size_t dcap = 0, dlen = 0;

    seq_t tmp = x;
    while (tmp > 0) {
        int d = (int)(tmp % (seq_t)3);
        if (dlen == dcap) {
            dcap = dcap ? dcap * 2 : 16;
            digits = (int *)realloc(digits, dcap * sizeof(int));
        }
        digits[dlen++] = d;
        tmp = tmp / (seq_t)3;
    }

    /* Process runs from the most significant digit. */
    bool expectOdd = true;               /* First run must be odd. */
    int curDigit = digits[dlen - 1];
    int curLen = 1;
    for (long i = (long)dlen - 2; i >= 0; --i) {
        int d = digits[i];
        if (d == curDigit) {
            ++curLen;
        } else {
            if ((curLen & 1) != (expectOdd ? 1 : 0)) {
                free(digits);
                return false;
            }
            expectOdd = !expectOdd;
            curDigit = d;
            curLen = 1;
        }
    }
    /* Check the last run. */
    if ((curLen & 1) != (expectOdd ? 1 : 0)) {
        free(digits);
        return false;
    }

    free(digits);
    return true;
}

/* Static cache for memoization. */
static seq_t *cache = NULL;
static size_t cacheSize = 0;
static size_t cacheCap = 0;
static seq_t lastChecked = 0;   /* Last integer that was tested. */

/* Ensure the cache array can hold at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cacheCap)
        return;
    size_t newCap = cacheCap ? cacheCap * 2 : 16;
    while (newCap < need)
        newCap *= 2;
    seq_t *newArr;
    if (cache == NULL)
        newArr = mem_new(seq_t, newCap);
    else
        newArr = mem_resize(seq_t, cache, newCap);
    cache = newArr;
    cacheCap = newCap;
}

/* Public function: compute a(n) with memoization. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1 */

    if ((size_t)n <= cacheSize)
        return cache[n - 1];

    while (cacheSize < (size_t)n) {
        ++lastChecked;
        if (passes_base3_parity(lastChecked)) {
            ensure_capacity(cacheSize + 1);
            cache[cacheSize++] = lastChecked;
        }
    }
    return cache[n - 1];
}
