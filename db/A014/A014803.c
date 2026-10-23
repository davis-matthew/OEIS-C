/*
 * A014803  Squares of even hexagonal pyramidal numbers.
 * https://oeis.org/A014803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((4*n+(-1)^n+5)*(4*n+(-1)^n+7)*(8*n+2*(-1)^n+9))^2/576
 * Range:     u8 none, u16 n <= 2, u32 n <= 21, u64 n <= 929, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work with positive integers only.  For even n  (-1)^n = +1,
       for odd n  (-1)^n = -1.  The three factors simplify to:       */
    /* n even : 4n+6 , 4n+8 , 8n+11                               */
    /* n odd  : 4n+4 , 4n+6 , 8n+7                               */

    seq_t nn = (seq_t)n;
    seq_t t4  = nn * (seq_t)4;   /* 4*n          */
    seq_t t8  = t4 * (seq_t)2;    /* 8*n          */

    seq_t t1, t2, t3;

    if ((n & (idx_t)1) == 0)      /* even n */
    {
        t1 = t4 + (seq_t)6;
        t2 = t4 + (seq_t)8;
        t3 = t8 + (seq_t)11;
    }
    else                         /* odd n  */
    {
        t1 = t4 + (seq_t)4;
        t2 = t4 + (seq_t)6;
        t3 = t8 + (seq_t)7;
    }

    seq_t prod = t1 * t2;
    prod = prod * t3;           /* product of the three factors */

    seq_t p = prod / (seq_t)24;  /* 24 divides the product exactly */
    return p * p;                /* square to get a(n)                 */
}
