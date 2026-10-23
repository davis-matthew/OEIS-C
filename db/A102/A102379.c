/*
 * A102379  a(n) is the minimal number of nodes in a height-ratio-balanced
 *          binary tree of order 1/2 and height n.
 * https://oeis.org/A102379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(n/2)) + 1, a(1) = 0
 * Range:     u8 n <= 21, u16 n <= 90, u32 n <= 620, u64 n <= 8421,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A102379: minimal number of nodes in a
 * height‑ratio‑balanced binary tree of order 1/2 and height n.
 * The result is memoised in a static table so that repeated calls
 * (in any order) are O(1) after the first computation.  The table
 * grows only as far as needed.  The implementation uses only integer
 * arithmetic and the standard library. */
#include <stdlib.h>

static seq_t *a_table = NULL;   /* memoisation table, 1‑based */
static idx_t a_max = 0;         /* maximal index that has been filled */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n=0, but return 0 */

    /* First call: allocate table up to n and initialise all entries. */
    if (a_table == NULL) {
        a_table = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!a_table)
            return (seq_t)0; /* allocation failure – unlikely in tests */

        a_table[0] = (seq_t)0;      /* unused */
        a_table[1] = (seq_t)0;      /* a(1) = 0 */

        for (idx_t i = 2; i <= n; ++i) {
            size_t idx      = (size_t)i;
            size_t half_idx = idx >> 1;   /* floor(i/2) */
            a_table[idx] = a_table[idx-1] + a_table[half_idx] + (seq_t)1;
        }
        a_max = n;
        return a_table[n];
    }

    /* If the requested n is already in the table, just return it. */
    if (n <= a_max)
        return a_table[n];

    /* Grow the table to accommodate n.  New entries are computed
     * using the recurrence and rely only on already known values. */
    seq_t *tmp = (seq_t*)realloc(a_table, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0; /* allocation failure – unlikely in tests */

    a_table = tmp;

    for (idx_t i = a_max + 1; i <= n; ++i) {
        size_t idx      = (size_t)i;
        size_t half_idx = idx >> 1;
        a_table[idx] = a_table[idx-1] + a_table[half_idx] + (seq_t)1;
    }
    a_max = n;
    return a_table[n];
}
