/*
 * A062723  Least common multiple (LCM) of the first n+1 terms of A000792.
 * https://oeis.org/A062723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*3^floor(n/3) for n >= 4; initial values a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=6
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 56, u64 n <= 119,
 *            u128 n <= 239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* For n >= 4 use the closed‑form: 4 * 3^(n/3) */
    idx_t k = n / 3;          /* floor(n/3) */
    seq_t result = (seq_t)4;  /* start with the factor 4 */

    /* Multiply by 3, k times */
    for (idx_t i = 0; i < k; ++i)
    {
        result *= (seq_t)3;
    }
    return result;
}
