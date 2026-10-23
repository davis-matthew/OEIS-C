/*
 * A037556  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,1.
 * https://oeis.org/A037556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A037556
   A(n) is the base-7 number whose first n digits are the repeating
   sequence 2,1,1,2,1,1,...  (digit d_k = 2 if k%3==0, else 1).
   Formula used:  a(1)=2; for n>1  a(n)=7*a(n-1)+d_{n-1}  with d_{n-1} as above.
   The implementation computes a(n) iteratively in O(n) time. */

seq_t a(idx_t n)
{
    /* Guard against offset 0; for n>=1 the loop runs correctly. */
    if (n == 0)
        return 0;

    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        val = val * 7 + ((i % 3) == 0 ? 2 : 1);
    }
    return val;
}
