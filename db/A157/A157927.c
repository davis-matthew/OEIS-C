/*
 * A157927  Joint-rank array of the numbers i^2+j^2, where i>=0, j>=0.
 * https://oeis.org/A157927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Joint‑rank array of the numbers i^2 + j^2, enumerated along
   the antidiagonals: (i, j) with i, j ≥ 0, ordered by s = i + j,
   and within each diagonal i increases.  The rank of a value v is
   the number of distinct sums not exceeding v.  a(n) returns the
   rank of the value at the n‑th position (1‑based) in this
   enumeration.  The implementation is O(n) for n ≤ 10 000 and
   works for any integer type seq_t. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Return the number of distinct sums i^2 + j^2 that are ≤ v. */
static seq_t rank_of(seq_t v)
{
    /* v is expected to be small for the problem sizes considered. */
    size_t limit = (size_t)v;
    bool *seen = (bool *)calloc(limit + 1, 1);
    if (!seen)
        return 0;                       /* allocation failure → 0 */

    for (idx_t a = 0; (seq_t)a * (seq_t)a <= v; ++a) {
        seq_t a_sq = (seq_t)a * (seq_t)a;
        seq_t rem  = v - a_sq;
        for (idx_t b = 0; (seq_t)b * (seq_t)b <= rem; ++b) {
            seq_t s = a_sq + (seq_t)b * (seq_t)b;
            seen[(size_t)s] = true;
        }
    }

    seq_t count = 0;
    for (size_t k = 0; k <= limit; ++k)
        if (seen[k])
            ++count;

    free(seen);
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* undefined for n < 1 */

    /* Determine the diagonal s such that
       s*(s+1)/2 < n ≤ (s+1)*(s+2)/2  */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 < n)
        ++s;

    idx_t prev = s * (s + 1) / 2;           /* elements before diagonal s */
    idx_t pos  = n - prev - 1;              /* 0‑based position within diagonal */

    idx_t i = pos;
    idx_t j = s - pos;

    seq_t val = (seq_t)i * (seq_t)i + (seq_t)j * (seq_t)j;
    return rank_of(val);
}
