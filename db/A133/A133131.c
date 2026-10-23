/*
 * A133131  a(4n) = 3n+1, a(4n+1) = -3n, a(4n+2) = -3n-3, a(4n+3) = 3n+2.
 * https://oeis.org/A133131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(4n) = 3n+1, a(4n+1) = -3n, a(4n+2) = -3n-3, a(4n+3) = 3n+2
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the integer quotient q = floor(n / 4) as idx_t */
    idx_t q   = n >> 2;          /* same as n / 4 for unsigned integers */
    seq_t sq  = (seq_t)q;        /* convert q to seq_t for arithmetic */

    switch (n & 3UL) {           /* n % 4 */
        case 0: /* 4q */
            return (seq_t)3 * sq + (seq_t)1;
        case 1: /* 4q + 1 */
            return -((seq_t)3 * sq);
        case 2: /* 4q + 2 */
            return -(((seq_t)3 * sq) + (seq_t)3);
        case 3: /* 4q + 3 */
            return (seq_t)3 * sq + (seq_t)2;
    }
    /* This point should never be reached */
    return 0;
}
