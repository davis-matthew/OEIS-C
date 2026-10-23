/*
 * A195148  Concentric 20-gonal numbers.
 * https://oeis.org/A195148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 + 2*(-1)^n - 2
 * Range:     u8 n <= 7, u16 n <= 114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented as a piecewise expression to avoid intermediate negatives:
   if n is even: a(n) = 5*n^2
   if n is odd : a(n) = 5*n^2 - 4
   This yields the same sequence: 0,1,20,41,... */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic that may overflow idx_t. */
    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t val = (seq_t)5 * sq;               /* 5*n^2 */

    /* Toggle by parity of n. */
    if ((n & 1ULL) == 0) {
        return val;                         /* even:   5*n^2          */
    } else {
        return val - (seq_t)4;              /* odd :   5*n^2 - 4      */
    }
}
