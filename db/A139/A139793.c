/*
 * A139793  Concatenation of a and b, where a is the reduced sum of the digits
 *          of n and b is reduced sum of the digits of A000217.
 * https://oeis.org/A139793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10*dr(n) + dr(n(n+1)/2) where dr(x) is the digital root
 *            of x (0 for x=0, otherwise 1+((x-1)%9))
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digital root of n */
    idx_t rem_n = n % 9;
    seq_t dr_n = (n == 0) ? 0 : (rem_n == 0 ? 9 : rem_n);

    /* digital root of T = n*(n+1)/2 */
    seq_t dr_T;
    if (n == 0) {
        dr_T = 0;
    } else {
        /* compute T mod 9 using (n*(n+1)/2) mod 9 = ((n mod 9)*(n+1 mod 9) * 5) mod 9
           because 2 has inverse 5 modulo 9. */
        idx_t a = rem_n;                /* n mod 9 */
        idx_t a_plus = (a + 1) % 9;     /* (n+1) mod 9 */
        idx_t prod = (a * a_plus) % 9;  /* product mod 9 */
        prod = (prod * 5) % 9;          /* multiply by inverse of 2 */
        dr_T = (prod == 0) ? 9 : prod;
    }

    return (dr_n * (seq_t)10) + dr_T;
}
