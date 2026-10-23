/*
 * A151989  a(n) = A001512(n)/24 = (5*n+1)*(5*n+2)*(5*n+3)*(5*n+4)/24.
 * https://oeis.org/A151989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n+1)*(5*n+2)*(5*n+3)*(5*n+4)/24
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 111, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute the formula directly. */
    seq_t m      = (seq_t)n;          /* n as seq_t                */
    seq_t five   = (seq_t)5 * m;      /* 5*n                       */
    seq_t prod   = (five + 1);         /* (5n+1)                    */
    prod *= (five + 2);                /* (5n+1)(5n+2)              */
    prod *= (five + 3);                /* (5n+1)(5n+2)(5n+3)        */
    prod *= (five + 4);                /* (5n+1)(5n+2)(5n+3)(5n+4)  */
    return prod / (seq_t)24;           /* divide by 24              */
}
