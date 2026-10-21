/*
 * A098338  Expansion of 1/sqrt(1-6x+13x^2).
 * https://oeis.org/A098338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum{k=0..floor(n/2)} binomial(n, k)*binomial(n-k,
 *            k)*3^n*(-9)^(-k). Recurrence:
 *            n*a(n)+3*(1-2*n)*a(n-1)+13*(n-1)*a(n-2)=0, a(0)=1, a(1)=3
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 18, i64 n <= 36, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a0 = 1;   /* a(n-2)   */
    seq_t a1 = 3;   /* a(n-1)   */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i)
    {
#ifdef __cplusplus
        /* Arbitrary‑precision or built‑in types with operator overloading. */
        a2 = ((a1 * ((seq_t)(2 * i) - 1) * 3) - (a0 * (seq_t)(i - 1) * 13)) / (seq_t)i;
#else
        /* Use 128‑bit integer for intermediate values to avoid overflow. */
        __int128 term1 = (__int128)a1 * (((__int128)2 * i) - 1) * 3;
        __int128 term2 = (__int128)a0 * (i - 1) * 13;
        a2 = (seq_t)((term1 - term2) / (__int128)i);
#endif
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
