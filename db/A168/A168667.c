/*
 * A168667  a(n) = n^7*(n^10 + 1)/2.
 * https://oeis.org/A168667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 * (n^10 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 14, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* Compute powers of n */
    seq_t n2  = sn * sn;           /* n^2  */
    seq_t n3  = n2 * sn;           /* n^3  */
    seq_t n4  = n2 * n2;           /* n^4  */
    seq_t n5  = n4 * sn;           /* n^5  */
    seq_t n7  = n4 * n3;           /* n^7  */
    seq_t n10 = n5 * n5;           /* n^10 */

    /* Choose the factor that is even to divide by 2 first,
       keeping intermediate results within the capacity of seq_t
       for the 64‑bit build when the final value fits. */
    seq_t result;
    if ((n7 & 1) == 0)              /* n^7 even */
        result = (n7 / 2) * (n10 + 1);
    else                            /* n^10 + 1 even */
        result = n7 * ((n10 + 1) / 2);

    return result;
}
