/*
 * A154292  Integers of the form m*(6*m -+ 1)/2.
 * https://oeis.org/A154292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k * (12k + 1) if n even, and a(n) = k * (12k - 1) if n
 *            odd, where k = (n + 1) / 2
 * Range:     u8 n <= 8, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = (12*n^2 - 10*(-1)^n*n + 12*n - 5*(-1)^n + 5) / 4. */

seq_t a(idx_t n)
{
    /* n is 1-based; k = floor((n+1)/2) */
    seq_t k = ((seq_t)n + 1) / 2;
    seq_t term;

    if (n & 1)          /* n is odd  → use 12k - 1  */
        term = 12 * k - 1;
    else                /* n is even → use 12k + 1  */
        term = 12 * k + 1;

    return k * term;
}
