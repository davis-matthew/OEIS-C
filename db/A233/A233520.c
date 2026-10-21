/*
 * A233520  The number of distinct values of x^x (mod n) - x for x in 0 < x <
 *          n.
 * https://oeis.org/A233520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of (x^x mod n) - x for 0 < x <
 *            n
 * Range:     u8 n <= 357, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* helpers */
#include "../utils/mod_pow.h"    /* provides: seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0 by definition */
        return (seq_t)0;

    /* The range of (x^x mod n) - x is [-n+1, n-2] => size 2*n-1 < 2*n */
    size_t size = 2 * n;          /* enough to cover all possible indices */
    bool *seen = (bool *)calloc(size, sizeof(bool));
    if (!seen)
        return (seq_t)0;          /* fallback in case of allocation failure */

    idx_t offset = n - 1;         /* shift negative values to non‑negative indices */

    for (idx_t x = 1; x < n; ++x) {
        seq_t pow = mod_pow((seq_t)x, x, (seq_t)n);
        int64_t diff = (int64_t)pow - (int64_t)x;   /* may be negative */
        idx_t idx  = (idx_t)(diff + offset);
        seen[idx] = true;
    }

    seq_t count = 0;
    for (size_t i = 0; i < size; ++i)
        if (seen[i]) ++count;

    free(seen);
    return count;
}
