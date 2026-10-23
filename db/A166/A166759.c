/*
 * A166759  Number of nX2 1..6 arrays containing at least one of each value,
 *          and all equal values connected.
 * https://oeis.org/A166759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/315)*(n-1)*(n-2)*(32*n^8 - 304*n^7 + 2744*n^6 -
 *            15880*n^5 + 62078*n^4 - 171871*n^3 + 319311*n^2 - 353880*n +
 *            170100)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1 or 2 the formula gives zero, but return explicitly. */
    if (n < 3)
        return (seq_t)0;

    seq_t ns = (seq_t)n;                     /* cast the index to seq_t */

    /* Compute successive powers of n up to n^8. */
    seq_t n2 = ns * ns;            /* n^2 */
    seq_t n3 = n2 * ns;            /* n^3 */
    seq_t n4 = n3 * ns;            /* n^4 */
    seq_t n5 = n4 * ns;            /* n^5 */
    seq_t n6 = n5 * ns;            /* n^6 */
    seq_t n7 = n6 * ns;            /* n^7 */
    seq_t n8 = n7 * ns;            /* n^8 */

    /* Evaluate the polynomial part of the formula. */
    seq_t poly =
        (seq_t)32  * n8
      - (seq_t)304 * n7
      + (seq_t)2744 * n6
      - (seq_t)15880 * n5
      + (seq_t)62078 * n4
      - (seq_t)171871 * n3
      + (seq_t)319311 * n2
      - (seq_t)353880 * ns
      + (seq_t)170100;

    /* Combine the pre‑factor (n-1)(n-2) and the polynomial, then
       multiply by 2 and divide by 315.  The division is exact. */
    seq_t tmp   = (ns - (seq_t)1) * (ns - (seq_t)2);
    seq_t numer = tmp * poly * ((seq_t)2);
    return numer / ((seq_t)315);
}
