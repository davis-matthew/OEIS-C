/*
 * A232229  a(1)=9; thereafter a(n) = 8*10^(n-1) + 8 + a(n-1).
 * https://oeis.org/A232229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 9; thereafter a(n) = 8*10^(n-1) + 8 + a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1;  for n ≤ 1 return the
       known value or 0 if n == 0 (unused). */
    if (n <= 1)
        return (n == 1 ? (seq_t)9 : 0);

    seq_t value = (seq_t)9;      /* a(1)                               */
    seq_t pow10 = (seq_t)10;     /* 10^(1) – will become 10^(k-1)      */

    /* Compute iteratively up to n.  For each step k we already know
       10^(k-1) stored in pow10. */
    for (idx_t k = 2; k <= n; ++k)
    {
        value = value + (seq_t)8 * pow10 + (seq_t)8;

        /* Update pow10 for the next iteration: 10^k.  No need to
           update when k == n. */
        if (k < n)
            pow10 *= (seq_t)10;
    }

    return value;
}
