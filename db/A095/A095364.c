/*
 * A095364  Number of walks of length n between two adjacent nodes in the
 *          cycle graph C_9.
 * https://oeis.org/A095364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) is the number of walks of length n between two *
 *            adjacent nodes in the cycle graph C_9. The sequence satisfies *
 *            a(n) = a(n-1) + 5*a(n-2) - 4*a(n-3) - 5*a(n-4) + 2*a(n-5) for
 *            n≥6. * Here we compute a(n) directly via a simple
 *            dynamic‑programming * recurrence on the 9‑vertex cycle
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
 
seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; return 0 for n==0
     * (value irrelevant below the offset). */
    if (n == 0)
        return 0;
 
    /* counts of walks ending at each of the 9 vertices after the current
     * number of steps.  cur[0] is the starting vertex. */
    seq_t cur[9] = {};   /* value‑initialize: all zero for built‑in types,
                           default constructor for class types */
    seq_t next[9] = {};  /* temporary storage for the next step */
 
    cur[0] = 1;  /* walk of length 0 starts at vertex 0 */
 
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 9; ++i) {
            int left  = (i + 8) % 9;  /* neighbour to the left  */
            int right = (i + 1) % 9;  /* neighbour to the right */
            next[i] = cur[left] + cur[right];
        }
        /* copy next into cur for the next iteration */
        for (int i = 0; i < 9; ++i)
            cur[i] = next[i];
    }
    /* vertex 1 is the neighbour of vertex 0, the value we need. */
    return cur[1];
}
