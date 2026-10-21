/*
 * A099403  Numbers n such that the sum of the decimal portions of the square
 *          root and cube root of n is greater than 1.
 * https://oeis.org/A099403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099403: a(n) = the n‑th integer k such that
   fractional part of sqrt(k) + fractional part of cbrt(k) > 1.
   Implementation: brute‑force search with double arithmetic for
   fractional‑part comparison.  A static cache of up to 200 000 terms
   is used and extended lazily.  This approach works for all n ≥ 1
   up to the cache size. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* Test whether m satisfies the condition. */
static bool good_uint64 (unsigned long long m)
{
    double d   = (double)m;
    double sq  = sqrt(d);
    double cb  = cbrt(d);

    /* fractional part of sqrt(m) -> sq - floor(sq) */
    double frac_s = sq - floor(sq);
    double frac_c = cb - floor(cb);

    return (frac_s + frac_c) > 1.0;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache (pre‑allocate a generous size). */
    static seq_t cache[200000];
    static size_t cache_len = 0;
    static unsigned long long last_checked = 0ULL;

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        ++last_checked;
        if (good_uint64(last_checked)) {
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
