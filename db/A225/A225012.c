/*
 * A225012  Number of 5 X n 0..1 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A225012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3628800)*n^10 + (1/80640)*n^9 + (1/3780)*n^8 * +
 *            (19/5760)*n^7 + (4633/172800)*n^6 + (331/2304)*n^5 * +
 *            (191249/362880)*n^4 + (24421/20160)*n^3 * + (10897/5600)*n^2 +
 *            (137/120)*n + 1 * * The numerator is an integer polynomial with
 *            coefficients: * 1, 45, 960, 11970, 97293, 521325, 191249...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 37, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for safety; the final result fits in seq_t
       when it is meant to.  */
    unsigned __int128 t   = (unsigned __int128)n;
    unsigned __int128 num = 1;                 /* coefficient of n^10   */

    /* Horner's rule: evaluate the integer numerator polynomial. */
    num = num * t + (unsigned __int128)45;          /* n^9 coefficient   */
    num = num * t + (unsigned __int128)960;          /* n^8 coefficient   */
    num = num * t + (unsigned __int128)11970;       /* n^7 coefficient   */
    num = num * t + (unsigned __int128)97293;       /* n^6 coefficient   */
    num = num * t + (unsigned __int128)521325;      /* n^5 coefficient   */
    num = num * t + (unsigned __int128)1912490;     /* n^4 coefficient   */
    num = num * t + (unsigned __int128)4395780;     /* n^3 coefficient   */
    num = num * t + (unsigned __int128)7061256;     /* n^2 coefficient   */
    num = num * t + (unsigned __int128)4142880;     /* n  coefficient   */
    num = num * t + (unsigned __int128)3628800;     /* constant term   */

    /* Divide by the common denominator 3628800. */
    return (seq_t)(num / (unsigned __int128)3628800);
}
