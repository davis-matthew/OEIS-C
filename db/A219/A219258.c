/*
 * A219258  Numbers k such that 27*k+1 is a square.
 * https://oeis.org/A219258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m * (27*m + 2) where m = (-1)^(n‑1) * floor(n/2)
 * Range:     u8 n <= 7, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This simplifies to: */
/*   if n is odd: a(n) = 27*(n/2)^2 + 2*(n/2) */
/*   if n is even: a(n) = 27*(n/2)^2 - 2*(n/2) */
seq_t a(idx_t n)
{
    /* Convert the integer half to seq_t to avoid promotion issues */
    seq_t half = (seq_t)(n / 2);
    seq_t sq = half * half;           /* (n/2)^2 */

    /* Compute depending on parity of n */
    if (n & 1)                         /* n is odd */
        return 27 * sq + 2 * half;
    else                               /* n is even */
        return 27 * sq - 2 * half;
}
