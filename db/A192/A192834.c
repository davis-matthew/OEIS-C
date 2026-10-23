/*
 * A192834  Molecular topological indices of the Mycielski graphs.
 * https://oeis.org/A192834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^n - 8)*(18 - 27*2^n + 14*3^n)/36, a(1)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    seq_t two_n  = pow2(n);
    seq_t three_n = pow3(n);

    /* For n > 1, the numerator is divisible by 36.
       We divide by 4 and 9 separately to avoid overflow. */
    seq_t part1 = ((seq_t)3 * two_n - (seq_t)8) / (seq_t)4;          /* (3*2^n - 8)/4 */
    seq_t part2 = ((seq_t)18 - (seq_t)27 * two_n + (seq_t)14 * three_n) / (seq_t)9; /* (18 - 27*2^n + 14*3^n)/9 */

    return part1 * part2;
}
