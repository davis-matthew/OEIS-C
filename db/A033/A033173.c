/*
 * A033173  Number of days in n years (n=2 is the first leap year).
 * https://oeis.org/A033173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((1461*n + 2) / 4)
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1).  For n = 0 we return 0, though the sequence starts at 1. */
    if (n == 0)
        return 0;

    /* Compute (1461 * n + 2) / 4 using integer arithmetic.  The division
       is truncating toward zero, which is exactly the floor for positive
       numbers.  All operations are performed in the type seq_t, which
       guarantees correct behaviour for unsigned 8/16/32/64/128 bits as
       well as arbitrary‑precision integers. */
    seq_t tmp = ((seq_t)1461) * (seq_t)n + (seq_t)2;
    return tmp / (seq_t)4;
}
