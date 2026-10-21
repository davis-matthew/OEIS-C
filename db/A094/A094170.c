/*
 * A094170  Number of quasi-triominoes in an n X n bounding box.
 * https://oeis.org/A094170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/32)*(6*n^4 - 12*n^3 + 32*n^2 - 58*n + 29 -
 *            (6*n-3)*(-1)^n)
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 389, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Special case: a(0) = 0) */
seq_t a(idx_t n)
{
    /* Handle offset 0 explicitly */
    if (n == 0)
        return 0;

    /* Use 128‑bit arithmetic to avoid overflow for n up to ~1e4 */
    unsigned __int128 nn = n;            /* cast idx_t to 128 bits */
    unsigned __int128 n2 = nn * nn;      /* n^2 */
    unsigned __int128 n3 = n2 * nn;      /* n^3 */
    unsigned __int128 n4 = n2 * n2;      /* n^4 */

    /* Compute the numerator in signed 128‑bit */
    signed __int128 numer = 0;
    numer += (signed __int128)6  * (signed __int128)n4;   /* 6*n^4 */
    numer -= (signed __int128)12 * (signed __int128)n3;   /* -12*n^3 */
    numer += (signed __int128)32 * (signed __int128)n2;   /* +32*n^2 */
    numer -= (signed __int128)58 * (signed __int128)nn;   /* -58*n   */
    numer += 29;                                        /* +29     */

    /* (-1)^n term: subtract (6*n-3)*(-1)^n */
    signed __int128 term = (signed __int128)6 * (signed __int128)nn - 3;  /* 6*n-3 */
    if (n & 1)         /* n odd -> (-1)^n = -1, so subtract negative -> + */
        term = -term;  /* for odd n, term = -(6*n-3) */
    numer -= term;     /* subtract (6*n-3)*(-1)^n */

    /* Divide by 32 – result is integer */
    numer /= 32;

    return (seq_t)numer;
}
