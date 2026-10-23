/*
 * A143488  "Fourth down, Extream [sic] between the two farthest Bells from
 *          it" in bell-ringing is a sequence of permutations p_1=(1,2,3,4),
 *          p_2=(1,2,4,3), .. which runs through all permutations of {1,2,3,4}
 *          with period 24; sequence gives position of bell 1 (the treble
 *          bell) in n-th permutation.
 * https://oeis.org/A143488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-12) + a(n-13) for n > 13. Initial values (n
 *            = 1 .. 13): 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 3, 3, 4. The sequence
 *            is periodic with period 24
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t tbl[24] = {0};
    static int initialized = 0;

    if (!initialized)
    {
        /* fill the first 13 terms */
        tbl[0]  = 1; tbl[1]  = 1; tbl[2]  = 1; tbl[3]  = 2; tbl[4]  = 2;
        tbl[5]  = 1; tbl[6]  = 1; tbl[7]  = 1; tbl[8]  = 2; tbl[9]  = 2;
        tbl[10] = 3; tbl[11] = 3; tbl[12] = 4;

        /* compute terms 14 .. 24 using the recurrence */
        for (int i = 13; i < 24; ++i)
        {
            tbl[i] = tbl[i - 1] - tbl[i - 12] + tbl[i - 13];
        }

        initialized = 1;
    }

    size_t idx = (size_t)((n - 1) % 24ULL);
    return tbl[idx];
}
