/*
 * A096941  Fourth column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+15)*(n+2)*(n+1)/6
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Factorise numerator and cancel division by 2 and 3 */
    seq_t a = nn + 15;
    seq_t b = nn +  2;
    seq_t c = nn +  1;

    /* Cancel factor 2 */
    if ((a & 1) == 0)
        a /= 2;
    else if ((b & 1) == 0)
        b /= 2;
    else
        c /= 2;

    /* Cancel factor 3 */
    if ((a % 3) == 0)
        a /= 3;
    else if ((b % 3) == 0)
        b /= 3;
    else
        c /= 3;

    /* Multiply remaining factors */
    return a * b * c;
}
