/*
 * A197909  Ceiling((n+1/n)^10).
 * https://oeis.org/A197909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n + 1/n)^10) = n^10 + 10*n^8 + 45*n^6 + 120*n^4 +
 *            210*n^2 + 252 + ceil((210*n^8 + 120*n^6 + 45*n^4 + 10*n^2 +
 *            1)/n^10)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 84, u128 n <= 7131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, but harmless */ 

    seq_t nn  = (seq_t)n;          /* cast index to seq_t once */
    seq_t n2  = nn * nn;           /* n^2          */
    seq_t n4  = n2 * n2;           /* n^4          */
    seq_t n6  = n4 * n2;           /* n^6          */
    seq_t n8  = n4 * n4;           /* n^8          */
    seq_t n10 = n8 * n2;           /* n^10         */

    /* Integer part of the expansion */
    seq_t int_part = n10
                   + (seq_t)10  * n8
                   + (seq_t)45  * n6
                   + (seq_t)120 * n4
                   + (seq_t)210 * n2
                   + (seq_t)252;

    /* Numerical part that determines the ceil (fractional part) */
    seq_t numer_frac = (seq_t)210 * n8
                     + (seq_t)120 * n6
                     + (seq_t)45  * n4
                     + (seq_t)10  * n2
                     + (seq_t)1;

    /* Ceil of the fractional part: (numer + denom - 1) / denom */
    seq_t ceil_frac = (numer_frac + n10 - (seq_t)1) / n10;

    return int_part + ceil_frac;
}
