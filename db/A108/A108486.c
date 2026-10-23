/*
 * A108486  a(n) = Sum_{k=0..floor(n/2)} binomial(2n-2k,2k)3^k*2^(n-k).
 * https://oeis.org/A108486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2n-2k,2k) * 3^k * 2^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n >= 4: a(n) = 4*a(n-1) + 8*a(n-2) + 24*a(n-3) - 36*a(n-4) */
/* Initial values: a(0)=1, a(1)=2, a(2)=10, a(3)=80 */

seq_t a(idx_t n)
{
    /* Handle small indices explicitly */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 10;
    if (n == 3) return 80;

    /* Use the recurrence relation */
    seq_t a0 = 1;  /* a(n-4)  */
    seq_t a1 = 2;  /* a(n-3)  */
    seq_t a2 = 10; /* a(n-2)  */
    seq_t a3 = 80; /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t an = 4 * a3 + 8 * a2 + 24 * a1 - 36 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
