/*
 * A096943  Sixth column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+25) * binomial(n+4, 4) / 5
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 5, a(1) = 26, a(2) = 81, ... */
seq_t a(idx_t n)
{
    /* Compute binomial(n+4,4) using 128‑bit intermediate to avoid overflow */
    unsigned __int128 tmp = 1;
    tmp *= (unsigned __int128)(n + 1);
    tmp *= (unsigned __int128)(n + 2);
    tmp *= (unsigned __int128)(n + 3);
    tmp *= (unsigned __int128)(n + 4);
    tmp /= 24;                       /* now tmp = binomial(n+4,4) */

    /* Multiply by (n+25) and divide by 5 */
    tmp *= (unsigned __int128)(n + 25);
    tmp /= 5;

    return (seq_t)tmp;
}
