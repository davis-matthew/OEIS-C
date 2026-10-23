/*
 * A167695  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 22*(a(n-1)+a(n-2)+...+a(n-14))
 *                 - 253*a(n-15) + p(n)
 * where p(0)=1, p(n)=2 for 1≤n≤14, p(15)=1, p(n)=0 for n>15.
 * Initial term: a(0)=1.
 */
seq_t a(idx_t n)
{
    /* hold the last 15 terms (index modulo 16). */
    seq_t buf[16] = {0};
    buf[0] = 1;                      /* a(0) */

    if (n == 0) return buf[0];

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;

        /* contributions from a(i-1) ... a(i-14) with coefficient 22 */
        if (i >= 1)
            sum += ((seq_t)22) * buf[(i - 1) % 16];
        if (i >= 2)
            sum += ((seq_t)22) * buf[(i - 2) % 16];
        if (i >= 3)
            sum += ((seq_t)22) * buf[(i - 3) % 16];
        if (i >= 4)
            sum += ((seq_t)22) * buf[(i - 4) % 16];
        if (i >= 5)
            sum += ((seq_t)22) * buf[(i - 5) % 16];
        if (i >= 6)
            sum += ((seq_t)22) * buf[(i - 6) % 16];
        if (i >= 7)
            sum += ((seq_t)22) * buf[(i - 7) % 16];
        if (i >= 8)
            sum += ((seq_t)22) * buf[(i - 8) % 16];
        if (i >= 9)
            sum += ((seq_t)22) * buf[(i - 9) % 16];
        if (i >= 10)
            sum += ((seq_t)22) * buf[(i - 10) % 16];
        if (i >= 11)
            sum += ((seq_t)22) * buf[(i - 11) % 16];
        if (i >= 12)
            sum += ((seq_t)22) * buf[(i - 12) % 16];
        if (i >= 13)
            sum += ((seq_t)22) * buf[(i - 13) % 16];
        if (i >= 14)
            sum += ((seq_t)22) * buf[(i - 14) % 16];

        /* contribution of a(i-15) with coefficient -253 */
        if (i >= 15)
            sum -= ((seq_t)253) * buf[(i - 15) % 16];

        /* add constant term p(i) */
        seq_t p = 0;
        if (i == 15)
            p = 1;
        else if (i <= 14)
            p = 2;

        buf[i % 16] = sum + p;
    }

    return buf[n % 16];
}
