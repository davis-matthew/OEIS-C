/*
 * A205141  The number s(j) such that n divides s(k)-s(j), where
 *          s(j)=j*(3*j-1) and k is the least positive integer for which such
 *          a j exists.
 * https://oeis.org/A205141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the generalized pentagonal number s(j)=j*(3*j-1)/2 where
 *            j is the first index whose value shares a residue modulo n with
 *            a later term; the function returns s(j)
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for re-use across calls */
    static seq_t *vals  = NULL;   /* values of s(j) */
    static idx_t *first = NULL;  /* first occurrence of each residue modulo n */
    static size_t cap_vals  = 0;
    static size_t cap_first = 0;

    /* Ensure we have enough space for vals[1..n+1] and first[0..n-1] */
    if (cap_vals < n + 2) {
        if (vals) free(vals);
        vals = (seq_t *)malloc((n + 2) * sizeof(seq_t));
        cap_vals = n + 2;
    }
    if (cap_first < n) {
        if (first) free(first);
        first = (idx_t *)malloc(n * sizeof(idx_t));
        cap_first = n;
    }

    /* clear first array for residues 0..n-1 */
    for (idx_t i = 0; i < n; ++i) first[i] = 0;

    /* Initial term j = 1 */
    seq_t s = 1;            /* s(1) = 1*(3*1-1)/2 = 1 */
    vals[1] = s;
    idx_t r = (idx_t)(s % (seq_t)n);
    first[r] = 1;

    /* Iterate j = 2 .. n+1 (first repeat must occur by n+1) */
    for (idx_t j = 2; j <= n + 1; ++j) {
        /* s(j) = s(j-1) + 3*(j-1) + 1  (derived from formula) */
        s += (seq_t)(3 * (j - 1) + 1);
        vals[j] = s;
        r = (idx_t)(s % (seq_t)n);
        if (first[r] == 0) {
            first[r] = j;
        } else {
            /* first[r] holds the earlier index j_repeat */
            return vals[first[r]];
        }
    }

    /* The following line should never be reached */
    return s;
}
