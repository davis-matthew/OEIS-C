/*
 * A084080  Length of lists created by n substitutions k -> range(k+1,1,-3)
 *          starting with {1}, counting down from k+1 to 1 step -3.
 * https://oeis.org/A084080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the list obtained after n substitutions of each
 *            * element k by the list range(k+1, 1, -3), starting from {1}. *
 *            The coefficient arrays are recomputed for each call; the
 *            algorithm * runs in O(n²) time with O(n) memory
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;              /* a(0) = 1 */

    /* The largest value that can appear after n steps is n+1.
       Allocate arrays of size n+3 to safely access indices up to n+1
       and the suffix sums. */
    const size_t sz = (size_t)n + 3;
    seq_t *cur  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *nxt  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *s0   = (seq_t*)calloc(sz, sizeof(seq_t)); /* suffix sums for residues 0 */
    seq_t *s1   = (seq_t*)calloc(sz, sizeof(seq_t)); /* suffix sums for residues 1 */
    seq_t *s2   = (seq_t*)calloc(sz, sizeof(seq_t)); /* suffix sums for residues 2 */

    if (!cur || !nxt || !s0 || !s1 || !s2) {
        /* Allocation failure: return zero as a safe fallback. */
        return (seq_t)0;
    }

    cur[1] = (seq_t)1;        /* initial list {1} */
    seq_t total = (seq_t)1;   /* length after current step */

    for (idx_t step = 1; step <= n; ++step) {
        size_t max_val = (size_t)step;      /* current list contains values ≤ step */

        /* Build suffix sums for all indices ≥ i. */
        s0[max_val + 1] = s1[max_val + 1] = s2[max_val + 1] = (seq_t)0;
        for (size_t i = max_val; i >= 1; --i) {
            s0[i] = s0[i + 1];
            s1[i] = s1[i + 1];
            s2[i] = s2[i + 1];
            switch (i % 3) {
                case 0: s0[i] += cur[i]; break;
                case 1: s1[i] += cur[i]; break;
                case 2: s2[i] += cur[i]; break;
            }
        }
        /* Set suffix sums at position 0 equal to those at 1. */
        s0[0] = s0[1];
        s1[0] = s1[1];
        s2[0] = s2[1];

        /* Compute next list counts and its total length. */
        seq_t new_total = (seq_t)0;
        for (size_t j = 1; j <= max_val + 1; ++j) {
            size_t idx = j - 1;              /* residue index for suffix lookup */
            seq_t val;
            switch (idx % 3) {
                case 0: val = s0[idx]; break;
                case 1: val = s1[idx]; break;
                case 2: val = s2[idx]; break;
            }
            nxt[j] = val;
            new_total += val;
        }
        total = new_total;

        /* Swap cur and nxt for the next iteration. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Clean up temporary arrays. */
    free(cur);
    free(nxt);
    free(s0);
    free(s1);
    free(s2);

    return total;
}
