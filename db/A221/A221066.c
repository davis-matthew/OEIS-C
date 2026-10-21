/*
 * A221066  Sum of neighbor maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to the sum mod 4 of
 *          their king-move neighbors in a random 0..3 n X 2 array.
 * https://oeis.org/A221066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 4*a(n-3) - 5*a(n-4) - 6*a(n-5)
 *            Initial values: a(1)=2, a(2)=8, a(3)=20, a(4)=56, a(5)=168
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Implementation of the recurrence for A221066.
   No dynamic allocation is needed because only the last five
   values are required to compute a(n).  The function is fully
   deterministic and works for any order of calls.  For the
   64‑bit build, intermediate values may overflow if n is large,
   but the arbitrary‑precision build handles them correctly. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Handle the first five terms directly. */
    switch ((idx_t)n) {
        case 1: return (seq_t)2;
        case 2: return (seq_t)8;
        case 3: return (seq_t)20;
        case 4: return (seq_t)56;
        case 5: return (seq_t)168;
    }

    /* Array holding the last five values of the sequence. */
    seq_t last[5] = { (seq_t)2, (seq_t)8, (seq_t)20, (seq_t)56, (seq_t)168 };

    /* Compute successive terms up to n. */
    for (idx_t k = 6; k <= n; ++k) {
        seq_t next = 2 * last[4] + 2 * last[3] + 4 * last[2]
                     - 5 * last[1] - 6 * last[0];
        /* Shift the window: discard the oldest value. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = next;
    }
    return last[4];
}
