/*
 * A123138  The n-th digit of a(n-1) gives the position of digit n in a(n),
 *          a(1)=263514.
 * https://oeis.org/A123138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 263514 if n is odd, 513642 if n is even
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A123138 – Periodic with period 2.  
   a(1) = 263514, a(2) = 513642, and a(n) = a(n-2).  */
seq_t a(idx_t n)
{
    /* Ensure n ≥ 1; behaviour for n < 1 is undefined. */
    idx_t parity = n & (idx_t)1;
    return parity ? (seq_t)263514 : (seq_t)513642;
}
