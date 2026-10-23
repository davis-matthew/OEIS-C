/*
 * A143484  "Fourth down, Extream [sic] between the two farthest Bells from
 *          it" in bell-ringing is a sequence of permutations p_1=(1,2,3,4),
 *          p_2=(1,2,4,3), .. which runs through all permutations of {1,2,3,4}
 *          with period 24; sequence gives number in position 1 of n-th
 *          permutation.
 * https://oeis.org/A143484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the first element of the n-th permutation in the plain
 *            changes cycle of four bells; the sequence repeats with period 24
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t first[24];

    if (!init) {
        init = true;
        /* Generate the 24 first elements by the plain changes (SJT) algorithm. */
        int perm[4] = {1, 2, 3, 4};          /* current permutation (1‑based values) */
        int dir[5];                         /* direction of each value (−1 = left, +1 = right) */
        int pos[5];                         /* current position of each value */

        for (int v = 1; v <= 4; ++v) {
            dir[v] = -1;                     /* initial direction: left */
            pos[v] = v - 1;                  /* position in perm array */
        }

        int idx = 0;                        /* index into first[] */
        while (idx < 24) {
            first[idx++] = perm[0];          /* store first element of current permutation */

            if (idx == 24)                  /* finished all 24 permutations */
                break;

            /* Find the largest mobile element. */
            int largest = 0;
            int largest_pos = -1;
            for (int v = 4; v >= 1; --v) {
                int ppos = pos[v];
                int d    = dir[v];
                int neigh = ppos + d;
                if (neigh >= 0 && neigh < 4 && v > perm[neigh]) {
                    largest = v;
                    largest_pos = ppos;
                    break;                  /* because we search in descending order */
                }
            }

            /* Swap largest with its neighbor in the direction it moves. */
            int neigh = largest_pos + dir[largest];
            int other = perm[neigh];
            perm[largest_pos] = other;
            perm[neigh]      = largest;
            pos[largest] = neigh;
            pos[other]  = largest_pos;

            /* Reverse direction of all elements larger than the moved one. */
            for (int v = largest + 1; v <= 4; ++v)
                dir[v] = -dir[v];
            /* The moved element keeps its direction */
        }
    }

    /* Compute index within the period (0‑based). */
    idx_t r = (n - 1) % 24;
    return first[r];
}
