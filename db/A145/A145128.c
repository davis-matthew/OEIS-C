/*
 * A145128  a(n) = 1 + (1200 + (634 + (225 + (85 + (15 + n)*n)*n)*n)*n)*n/720.
 * https://oeis.org/A145128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (1200 + (634 + (225 + (85 + (15 +
 *            n)*n)*n)*n)*n)*n/720
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t
a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (seq_t)15;          /* (15 + n)                     */
    t = t + nn;
    t = t * nn;                    /* (15 + n)*n                    */
    t = t + (seq_t)85;             /* 85 + (15 + n)*n              */
    t = t * nn;                    /* n*(85 + (15 + n)*n)           */
    t = t + (seq_t)225;            /* 225 + n*(85 + ...)            */
    t = t * nn;                    /* n*(225 + ...)                 */
    t = t + (seq_t)634;            /* 634 + n*(225 + ...)           */
    t = t * nn;                    /* n*(634 + ...)                 */
    t = t + (seq_t)1200;           /* 1200 + n*(634 + ...)          */
    t = t * nn;                    /* n*(1200 + ...)                */
    t = t / (seq_t)720;            /* integer division, exact       */
    t = t + (seq_t)1;              /* +1 to finish the formula      */
    return t;
}
