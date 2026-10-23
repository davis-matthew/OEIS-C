/*
 * A168652  Integers that can be generated with a C/C++ expression that is
 *          three or more characters shorter than their decimal
 *          representation.
 * https://oeis.org/A168652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 100000 * n
 * Range:     u8 none, u16 none, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) * ((seq_t)100000);
}
