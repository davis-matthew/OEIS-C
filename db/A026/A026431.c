/*
 * A026431  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for
 *          1<=i<j<=n, with initial terms 1,2.
 * https://oeis.org/A026431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = least positive integer > a(n‑1) that is NOT of the
 * form a(i)*a(j) – 1 for any 1 ≤ i < j ≤ n.  The first terms are
 * a(1) = 1, a(2) = 2.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_ensure_capacity.h"

/* ensure a[0] is a dummy so that a[1] … a[seq_count] are the sequence */
static inline bool seq_contains(const seq_t *seq, size_t count, seq_t value)
{
    size_t lo = 1, hi = count;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = seq[mid];
        if (midv == value) return true;
        if (midv < value) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;          /* sequence array, index starts at 1 */
    static size_t seq_count = 0;       /* number of terms computed */
    static size_t seq_capacity = 0;    /* allocated capacity of seq */

    /* initialise first two terms on the very first call */
    if (seq_count == 0) {
        seq_ensure_capacity(&seq, &seq_capacity, 8);
        seq[0] = (seq_t)0;            /* dummy */
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)2;
        seq_count = 2;
    }

    /* n is unsigned; if callers give n < 1 return 0 (undefined) */
    if (n < 1) return (seq_t)0;

    /* compute up to n if necessary */
    for (size_t i = seq_count + 1; i <= (size_t)n; ++i) {
        seq_t candidate = seq[i-1] + 1;   /* smallest number > previous term */

        for (;;) {
            seq_t val = candidate + 1;   /* test val = a(i)+1 for forbidden product */
            bool forbidden = false;

            for (size_t p = 1; p <= seq_count; ++p) {
                seq_t x = seq[p];
                if (x > val) break;          /* x too large to divide val */
                if (val % x == 0) {
                    seq_t y = val / x;
                    if (y == x) continue;   /* the two factors must come from different indices */
                    if (seq_contains(seq, seq_count, y)) {
                        forbidden = true;
                        break;
                    }
                }
            }

            if (!forbidden) break;         /* candidate accepted */
            ++candidate;                   /* try next integer */
        }

        seq_ensure_capacity(&seq, &seq_capacity, i + 1);
        seq[i] = candidate;
        seq_count = i;
    }

    return seq[(size_t)n];
}
