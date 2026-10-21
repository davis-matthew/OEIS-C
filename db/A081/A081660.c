/*
 * A081660  n+A001045(n+1).
 * https://oeis.org/A081660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1)/3 + n + (-1)^n/3 Recurrence used: b(0)=1; for n≥1
 *            if n is even: b(n) = 2*b(n-1) + 1 if n is odd : b(n) = 2*b(n-1)
 *            – 1 Then a(n) = b(n) + n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t b = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((i & 1) == 0)   /* even i */
            b = 2 * b + 1;
        else                /* odd i */
            b = 2 * b - 1;
    }
    return b + n;
}
