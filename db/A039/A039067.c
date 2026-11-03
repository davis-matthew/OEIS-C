/*
 * A039067  Numbers whose base-8 representation has the same number of 3's and
 *          4's.
 * https://oeis.org/A039067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search counting base‑8 digits. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Count the digits 3 and 4 in the base‑8 representation of X. */
static void count_34(idx_t x, int *c3, int *c4)
{
    int d3 = 0, d4 = 0;
    idx_t tmp = x;
    if (tmp == 0) {
        /* Representation of 0 is the single digit 0, so c3 = c4 = 0. */
    } else {
        /* 8 = 2^3, so division and modulus by 8 can be done by shifts. */
        while (tmp) {
            int d = (int)(tmp & 0x7);  /* tmp % 8 */
            if (d == 3)
                d3++;
            else if (d == 4)
                d4++;
            tmp >>= 3;                 /* tmp /= 8 */
        }
    }
    *c3 = d3;
    *c4 = d4;
}

/* Cache for the sequence.  The cache is expanded on demand. */
static seq_t *cache = NULL;     /* array of stored terms                      */
static size_t cache_len = 0;     /* number of terms currently cached           */
static size_t cache_cap = 0;     /* allocated capacity of the cache            */

/* Ensure the cache has a capacity of at least NEED.  The capacity is
   doubled each time it is exceeded.  The cached terms are left untouched. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    if (new_cap < need)
        new_cap = need;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Return the n‑th term of OEIS A039067.  Offset = 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined behaviour below offset */
    /* Generate terms until we have at least n terms cached. */
    idx_t cur = 0;
    if (cache_len > 0)
        cur = (idx_t)cache[cache_len - 1] + 1;  /* continue from last cached term */
    while (cache_len < n) {
        int c3 = 0, c4 = 0;
        count_34(cur, &c3, &c4);
        if (c3 == c4) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)cur;
        }
        cur++;
    }
    return cache[n - 1];
}
