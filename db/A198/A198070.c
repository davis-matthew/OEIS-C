/*
 * A198070  a(n) = round((n+1/n)^5).
 * https://oeis.org/A198070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n+1/n)^5)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute all powers of n using 128‑bit integers to avoid overflow. */
    unsigned __int128 nn = (unsigned __int128)n;      /* n          */
    unsigned __int128 n2 = nn * nn;                  /* n^2        */
    unsigned __int128 n3 = n2 * nn;                  /* n^3        */
    unsigned __int128 n4 = n2 * n2;                  /* n^4        */
    unsigned __int128 n5 = n4 * nn;                  /* n^5        */

    /* Integer part: n^5 + 5*n^3 + 10*n */
    unsigned __int128 int_part = n5 + 5 * n3 + 10 * nn;

    /* Fractional part: (10*n^4 + 5*n^2 + 1) / n^5 */
    unsigned __int128 p = 10 * n4 + 5 * n2 + 1;     /* numerator  */
    unsigned __int128 q = n5;                      /* denominator*/

    /* Round the fraction: floor((p + q/2) / q) */
    unsigned __int128 round_frac = (p + q / 2) / q;

    unsigned __int128 result = int_part + round_frac;
    return (seq_t)result;
}
