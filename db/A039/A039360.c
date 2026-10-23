/*
 * A039360  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 4's.
 * https://oeis.org/A039360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A039360: Numbers whose base‑9 representation has the same
    non‑zero number of 0's and 4's.  The function computes a(n) by
    brute‑force search and caches the terms for repeated queries. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc */

static void
ensure_capacity(size_t need, seq_t **arr, size_t *cap)
{
    if (*cap < need) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 1;
        while (newcap < need)
            newcap <<= 1;
        *arr = seq_realloc(*arr, *cap, newcap);
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t size = 0;
    static size_t cap  = 0;
    static seq_t next  = 1;  /* number to test next */

    while (size < (size_t)n) {
        seq_t val = next++;
        size_t cnt0 = 0, cnt4 = 0;
        seq_t tmp = val;
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % 9);
            if (digit == 0)    ++cnt0;
            else if (digit == 4) ++cnt4;
            tmp /= 9;
        }
        if (cnt0 == cnt4 && cnt0 > 0) {
            ensure_capacity(size + 1, &terms, &cap);
            terms[size++] = val;
        }
    }
    return terms[n - 1];
}
