/*
 * A164965  Cumulative sums of A010892.
 * https://oeis.org/A164965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + 1; a(0)=1, a(1)=2. The sequence is
 *            periodic of length 6: n mod 6: 0 1 2 3 4 5 a(n): 1 2 2 1 0 0
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {(seq_t)1, (seq_t)2, (seq_t)2,
                                    (seq_t)1, (seq_t)0, (seq_t)0};
    return pattern[(size_t)(n % 6)];
}
