/*
 * A096285  Denominator of the ratio of the preceding two terms.
 * https://oeis.org/A096285
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A096285 – denominator of a(n)=a(n-1)/a(n-2) with a(1)=1, a(2)=2.
   The denominators are periodic with period 6:
   for n mod 6 == 0 or 5 the denominator is 2, otherwise it is 1. */
seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined by the offset,
       but returning 1 keeps the function well‑defined. */
    if (n < 3)
        return (seq_t)1;

    idx_t r = n % 6;
    return (r == 5 || r == 0) ? (seq_t)2 : (seq_t)1;
}
