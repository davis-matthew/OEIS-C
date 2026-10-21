/*
 * A050623  Smallest n-digit number divisible by 3^n.
 * https://oeis.org/A050623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3^n * ceil(10^(n-1) / 3^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest n‑digit number divisible by 3^n */
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    seq_t pow3 = 1;        /* 3^n */
    seq_t pow10 = 1;       /* 10^(n‑1) */
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;
    for (idx_t i = 0; i < n - 1; ++i)
        pow10 *= (seq_t)10;

    seq_t q = pow10 / pow3;   /* floor(10^(n-1) / 3^n) */
    seq_t r = pow10 % pow3;   /* remainder */

    if (r == 0)
        return pow10;               /* already divisible */
    else
        return (q + 1) * pow3;      /* next multiple of 3^n */
}
