/*
 * A116594  a^n*b^m ordering of IChing symbol numbers.
 * https://oeis.org/A116594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned, obsc
 * Formula:   a(n) is the nth term of the IChing symbol number ordering sorted
 *            by the number of yin (b) symbols (weight 0..6) and, for equal
 *            weight, by the symbol number in increasing order
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Full permutation of the 64 symbol numbers.  The sequence is
       obtained by grouping symbol numbers with the same number of
       yin symbols, sorting each group ascendingly, and then concatenating
       the groups in order of increasing yin count.  */
    static const seq_t seq[64] = {
        1, 9,10,13,14,43,44,5,6,25,26,28,30,33,34,37,38,49,50,57,58,61,
       11,12,17,18,21,22,31,32,41,42,47,48,53,54,55,56,59,60,63,64,
        3, 4,19,20,27,29,35,36,39,40,45,46,51,52,62, 7, 8,15,16,23,24,2
    };

    /* The sequence is defined only for 0 ≤ n < 64.  For larger indices
       the function returns 0 (undefined). */
    return (n < 64) ? seq[(size_t)n] : 0;
}
