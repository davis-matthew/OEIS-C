/*
 * A103948  a(n) is the number of distinct n-th powers of functions {1, 2, 3}
 *          -> {1, 2, 3}.
 * https://oeis.org/A103948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103948: number of distinct n‑th powers of all functions
   f:{1,2,3}->{1,2,3}.  The function enumerates the 27 possible
   functions, composes them n times (modularly by inspection of the
   resulting map), and counts how many different resulting functions
   appear.  This is a direct implementation of the definition and
   works for any n ≥ 0. */

#include <stdint.h>

static const uint8_t all_funcs[27][3] = {
    {0,0,0}, {0,0,1}, {0,0,2},
    {0,1,0}, {0,1,1}, {0,1,2},
    {0,2,0}, {0,2,1}, {0,2,2},
    {1,0,0}, {1,0,1}, {1,0,2},
    {1,1,0}, {1,1,1}, {1,1,2},
    {1,2,0}, {1,2,1}, {1,2,2},
    {2,0,0}, {2,0,1}, {2,0,2},
    {2,1,0}, {2,1,1}, {2,1,2},
    {2,2,0}, {2,2,1}, {2,2,2}
};

seq_t a(idx_t n)
{
    /* Mark which n‑th power maps have already appeared.  The
       number of possible maps is 3^3 = 27, so we can use a small
       boolean array. */
    unsigned char seen[27] = {0};
    unsigned char result_key;
    unsigned char res_map[3];
    unsigned int count = 0;   /* will not exceed 27 */
    unsigned int i, j, k, t;

    for (t = 0; t < 27; ++t) {      /* for each function f */
        const uint8_t *f = all_funcs[t];
        /* Compute f^n for each of the 3 domain elements. */
        for (i = 0; i < 3; ++i) {
            uint8_t v = (uint8_t)i;
            /* If n is zero, the power is the identity map. */
            if (n == 0) {
                res_map[i] = v;
                continue;
            }
            /* Use simple iteration – domain size is only 3. */
            for (k = 0; k < n; ++k) {
                v = f[v];
            }
            res_map[i] = v;
        }
        /* Encode the resulting function as an integer 0..26. */
        result_key = res_map[0] + 3 * res_map[1] + 9 * res_map[2];
        if (!seen[result_key]) {
            seen[result_key] = 1;
            ++count;
        }
    }
    return (seq_t)count;
}
