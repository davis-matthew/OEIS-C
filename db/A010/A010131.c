/*
 * A010131  Continued fraction for sqrt(33).
 * https://oeis.org/A010131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 9871, u16 n <= 9871, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A010131 – Continued fraction for sqrt(33)
 * 
 * Formula: a(0) = 5; for n >= 1 let e be the exponent of 2 in n
 *          a(n) = 10          if e >= 2  (n divisible by 4)
 *          a(n) =  2          if e = 1  (n even but not divisible by 4)
 *          a(n) =  1          if e = 0  (n odd)
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    /* n is unsigned, bitwise ops are safe */
    if ((n & 1) == 1)                 /* odd */
        return (seq_t)1;
    else if ((n & 3) == 2)            /* 2 mod 4 */
        return (seq_t)2;
    else                              /* divisible by 4 */
        return (seq_t)10;
}
