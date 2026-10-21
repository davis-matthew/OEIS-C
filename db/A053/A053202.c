/*
 * A053202  Pascal's triangle (excluding first, last two elements of each row)
 *          read by rows, row n read mod n.
 * https://oeis.org/A053202
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, nice, tabl
 * Formula:   a(n) = binomial(row, k) mod row, where row is the smallest
 *            integer with (row-3)*(row-2)/2 >= n-3, and k = (n-3) -
 *            (row-4)*(row-3)/2 + 1
 * Range:     u8 n <= 4756, u16 n <= 4756, u32 n <= 4756, u64 n <= 4756,
 *            u128 n <= 4756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple Euclidean GCD for 64‑bit values. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the requested term of A053202. */
seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 4.  For smaller n we return 0. */
    if (n < 4) return 0;

    /* Position in the flattened triangle, 1‑based. */
    idx_t pos = n - 3;

    /* Find the row R such that sum_{r=4..R} (r-3) >= pos. */
    idx_t low  = 4;
    idx_t high;
    if (pos > (UINT64_MAX - 4)) high = UINT64_MAX;
    else                        high = pos + 4;          /* safe upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t pref = (mid - 3) * (mid - 2) / 2;          /* (mid-3)*(mid-2)/2 */
        if (pref >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;

    /* Determine position inside this row. */
    idx_t before = (row - 4) * (row - 3) / 2;           /* entries up to previous row */
    idx_t offset = pos - before;                        /* 1‑based offset within row */
    idx_t k = offset + 1;                               /* column index (k >= 2) */

    /* Compute C(row, k) mod row.  Use cancellation of denominators to stay
       within 64‑bit integers, then reduce modulo. */
    idx_t denom_size = k;                               /* size of denominator array */
    uint64_t *den = (uint64_t*)malloc(denom_size * sizeof(uint64_t));
    if (!den) return 0;                                 /* allocation failure */

    for (idx_t i = 0; i < denom_size; ++i)
        den[i] = i + 1;                                 /* denominators 1..k */

    seq_t result = 1;                                   /* accumulating product */

    for (idx_t i = 0; i < denom_size; ++i) {
        uint64_t num = row - k + i + 1;                 /* numerator term */

        /* Cancel common factors with all remaining denominators. */
        for (idx_t j = 0; j < denom_size; ++j) {
            if (den[j] == 1) continue;
            uint64_t g = gcd_u64(num, den[j]);
            if (g > 1) {
                num   /= g;
                den[j] /= g;
            }
        }

        /* Multiply the remaining part of the numerator modulo 'row'. */
        uint64_t mod_row = (uint64_t)row;
        uint64_t temp   = (unsigned long long)result *
                          (unsigned long long)(num % mod_row);
        result = (seq_t)(temp % mod_row);
    }

    free(den);
    return result;
}
