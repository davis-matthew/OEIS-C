/*
 * A042778  Numerators of continued fraction convergents to sqrt(920).
 * https://oeis.org/A042778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 182*a(n-2) - a(n-4) Initial values: a(0)=30, a(1)=91,
 *            a(2)=5490, a(3)=16561 OEIS A042778: Numerators of continued
 *            fraction convergents to sqrt(920)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a[4] = { (seq_t)30, (seq_t)91, (seq_t)5490, (seq_t)16561 };

    if (n < 4) return a[n];

    for (idx_t i = 4; i <= n; ++i)
    {
        /* recurrence: a(i) = 182*a(i-2) - a(i-4) */
        a[i % 4] = 182 * a[(i - 2) % 4] - a[(i - 4) % 4];
    }
    return a[n % 4];
}
