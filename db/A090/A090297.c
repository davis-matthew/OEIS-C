/*
 * A090297  a(n) = K_5(n) = Sum_{k>=0} A090285(5,k)*2^k*binomial(n,k). a(n) =
 *          2*(2*n^5+45*n^4+360*n^3+1215*n^2+1528*n+315)/15.
 * https://oeis.org/A090297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(2*n^5+45*n^4+360*n^3+1215*n^2+1528*n+315)/15
 *            Equivalently a(n) =
 *            (4*n^5+90*n^4+720*n^3+2430*n^2+3056*n+630)/15. This
 *            implementation evaluates the polynomial via Horner's rule using
 *            an unsigned 128‑bit intermediate to avoid overflow on 64‑bit
 *            builds for the n‑range tested. The result is...
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for safe intermediate results      */
    unsigned __int128 P = 4;
    P = P * (unsigned __int128)n + 90;
    P = P * (unsigned __int128)n + 720;
    P = P * (unsigned __int128)n + 2430;
    P = P * (unsigned __int128)n + 3056;
    P = P * (unsigned __int128)n + 630;
    P = P / 15;
    return (seq_t)P;
}
