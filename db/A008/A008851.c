/*
 * A008851  Congruent to 0 or 1 mod 5.
 * https://oeis.org/A008851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*((n-1)/2) for n odd; a(n) = 5*(n/2) - 4 for n even.
 *            (Same as a(n) = (10*n - 13 - 3*(-1)^n)/4.)
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; we rely on the sequence being defined only for n ≥ 1. */
    if (n & 1)  /* n is odd */
        return (seq_t)5 * ( (seq_t)(n - 1) / 2 );
    else        /* n is even */
        return (seq_t)5 * ( (seq_t)(n / 2) ) - 4;
}
