/*
 * A088039  Smallest k such that k^3 == 1 (mod some n-th power), k > 1.
 * https://oeis.org/A088039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 for n = 1 4 for n = 2 2ⁿ + 1 for n ≥ 3 (Conjectured
 *            formula for A088039; matches all known terms)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)4;
    /* For n ≥ 3 compute 2ⁿ + 1 as (1 << n) + 1.
       Shift is defined for all supported seq_t types.           */
    return ((seq_t)1 << n) + (seq_t)1;
}
