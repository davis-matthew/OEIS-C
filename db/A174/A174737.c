/*
 * A174737  a(n) = a(n-1) - a(n-2), with a(0)=2, a(1)=-1.
 * https://oeis.org/A174737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2), a(0)=2, a(1)=-1
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 6: 2, -1, -3, -2, 1, 3 */

seq_t a(idx_t n)
{
    /* static period array – values fit in any seq_t implementation */
    static const seq_t period[6] = { 2, -1, -3, -2, 1, 3 };
    
    /* modulo 6 gives the appropriate index into the period */
    idx_t r = n % 6;
    return period[r];
}
