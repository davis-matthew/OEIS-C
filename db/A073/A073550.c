/*
 * A073550  Number of Fibonacci numbers F(k), k <= 10^n, which end in 1.
 * https://oeis.org/A073550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = (4*10^(n-1)+2)/3 (n >= 1), a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)3;          /* a(1) = 3; a(0) not used */

    /* Compute x = 10^(n-1) */
    seq_t x = 1;
    for (idx_t i = 1; i < n; ++i)
        x *= (seq_t)10;           /* 10^(n-1) fits in seq_t for all values tested */

    /* a(n) = x + (x+2)/3.  The division is exact because 10^(k)≡1 (mod 3). */
    seq_t inc = (x + 2) / 3;
    return x + inc;
}
