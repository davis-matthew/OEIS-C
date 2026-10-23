/*
 * A128834  Periodic sequence 0,1,1,0,-1,-1,...
 * https://oeis.org/A128834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n+1) = a(n) - a(n-1) with a(0)=0, a(1)=1 (period 6)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 6) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)0;
        case 4: return (seq_t)-1;
        case 5: return (seq_t)-1;
    }
    return (seq_t)0; /* never reached */
}
