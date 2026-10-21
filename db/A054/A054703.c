/*
 * A054703  Number of distinct powers of 2 modulo n.
 * https://oeis.org/A054703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Number of distinct powers of 2 modulo n
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1 specially to avoid
       allocating an array of size 1 unnecessarily. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Allocate a visited array of n bytes.  The size_t cast is
       safe for the intended use (n up to ~10⁴ in the tests). */
    unsigned char *visited = (unsigned char *)malloc((size_t)n);
    if (!visited) {
        /* Allocation failure – return 0 (should not happen in test). */
        return (seq_t)0;
    }

    /* Mark all entries as not visited initially. */
    for (idx_t i = 0; i < n; ++i) {
        visited[i] = 0;
    }

    seq_t curr = (seq_t)1 % (seq_t)n;
    size_t count = 1;
    visited[(idx_t)curr] = 1;

    /* Iterate until a repeat residue is produced. */
    while (1) {
        curr = (curr * 2) % (seq_t)n;
        idx_t idx = (idx_t)curr;
        if (visited[idx]) {
            break;
        }
        visited[idx] = 1;
        ++count;
    }

    free(visited);
    return (seq_t)count;
}
