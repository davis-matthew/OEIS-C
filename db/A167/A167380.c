/*
 * A167380  a(1)=1, a(2)=2, and continued periodically with 4, 5, 1, -4, -5,
 *          -1 .
 * https://oeis.org/A167380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167380:
   a(1)=1, a(2)=2, and thereafter periodic with period 6:
   4,5,1,-4,-5,-1 (starting at n=3). */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    /* For n >= 3 use the periodic pattern */
    idx_t r = n % 6;          /* n modulo 6 */
    switch (r)
    {
        case 3: return 4;    /* 6k-3  */
        case 4: return 5;    /* 6k-2  */
        case 5: return 1;    /* 6k-1  */
        case 0: return -4;   /* 6k    */
        case 1: return -5;   /* 6k+1  */
        case 2: return -1;   /* 6k+2  */
    }
    return 0;                 /* unreachable but keeps compiler happy */
}
