/*
 * A206728  Number of n X 2 0..2 arrays avoiding the pattern z-1 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A206728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A206727(n)^2, where A206727(n) counts length‑n strings
 *            over the alphabet {0,1,2} that avoid the patterns 0 0 1 and 1 1
 *            2. Initial values: A206727(1)=3, A206727(2)=9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == (idx_t)1) return (seq_t)9;   /* 3^2 */
    if (n == (idx_t)2) return (seq_t)81;  /* 9^2 */

    /* States: the last two symbols (x,y) of a partial word.
       Encode a state as x*3 + y (0 ≤ x,y ≤ 2).  There are 9 states. */
    seq_t curr[9] = {0};
    seq_t next[9] = {0};

    /* Length‑2 initialization: every pair of symbols is allowed. */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            curr[a * 3 + b] = (seq_t)1;

    /* Build up words of length n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Clear the next state array. */
        for (int i = 0; i < 9; ++i) next[i] = (seq_t)0;

        /* Transition from every state by adding a new symbol c. */
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b) {
                seq_t val = curr[a * 3 + b];
                if (val == (seq_t)0) continue;
                for (int c = 0; c < 3; ++c) {
                    /* Skip forbidden patterns 0 0 1 and 1 1 2. */
                    if (a == 0 && b == 0 && c == 1) continue;
                    if (a == 1 && b == 1 && c == 2) continue;
                    next[b * 3 + c] += val;
                }
            }

        /* Move to the next step. */
        for (int i = 0; i < 9; ++i) curr[i] = next[i];
    }

    /* Sum all final states to get A206727(n). */
    seq_t count = 0;
    for (int i = 0; i < 9; ++i) count += curr[i];

    /* Return its square: a(n) = A206727(n)^2. */
    return count * count;
}
