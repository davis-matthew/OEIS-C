/*
 * A207094  Number of 0..2 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its two previous neighbors modulo 3.
 * https://oeis.org/A207094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A207094: number of length‑n arrays over {0,1,2} such that
   each element is at least the sum (mod 3) of its two previous elements.
   For positions 0 and 1 the missing preceding elements are taken to be 0,
   which forces x[1] ≥ x[0].
   Computation uses a 9‑state DP over the last two symbols.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – not used but harmless */
    if (n == 1) return (seq_t)3;   /* any single symbol (0,1,2) */

    /* dp_prev and dp_cur hold counts for states (a,b), flattened as a*3+b */
    seq_t dp_prev[9] = {0}, dp_cur[9];
    /* initialise for length 2 (indices 0 and 1) where only b≥a is allowed */
    for (int a = 0; a <= 2; ++a)
        for (int b = 0; b <= 2; ++b)
            if (b >= a)
                dp_prev[a * 3 + b] = (seq_t)1;

    /* iterate for positions i = 2 .. n-1 */
    for (idx_t pos = 2; pos < n; ++pos)
    {
        for (int i = 0; i < 9; ++i) dp_cur[i] = (seq_t)0;
        for (int pa = 0; pa <= 2; ++pa)
            for (int qa = 0; qa <= 2; ++qa)
            {
                seq_t count = dp_prev[pa * 3 + qa];
                if (!count) continue;
                int threshold = (pa + qa) % 3;
                for (int c = threshold; c <= 2; ++c)
                    dp_cur[qa * 3 + c] += count;
            }
        /* copy dp_cur to dp_prev for next iteration */
        for (int i = 0; i < 9; ++i) dp_prev[i] = dp_cur[i];
    }

    /* sum all states to get the total number of arrays of length n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 9; ++i)
        total += dp_prev[i];
    return total;
}
