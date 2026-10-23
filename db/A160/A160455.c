/*
 * A160455  Number of triangles that can be built from rods with lengths
 *          1,2,...,n by using and concatenating all rods.
 * https://oeis.org/A160455
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A005044(n*(n+1)/2) where for perimeter p: if p mod 2 ==
 *            0: A005044(p) = (p*p + 24)/48 else : A005044(p) = ((p+3)*(p+3) +
 *            24)/48 (This rounds p²/48 for even p and (p+3)²/48 for odd p.)
 * Range:     u8 n <= 14, u16 n <= 59, u32 n <= 371, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)               /* a(3) = 0 by definition */
        return 0;

    /* Total perimeter of rods 1..n */
    seq_t P = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Count of integer triangles with perimeter P */
    if ((P & 1) == 0)        /* P even: use the even‑parity formula */
        return ((P * P) + 24) / 48;
    else                     /* P odd: use the odd‑parity formula */
        return ((((P + 3) * (P + 3)) + 24) / 48);
}
