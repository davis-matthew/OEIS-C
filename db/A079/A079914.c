/*
 * A079914  Solution to the Dancing School Problem with 9 girls and n+9 boys:
 *          f(9,n).
 * https://oeis.org/A079914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 -27*n^8 + 414*n^7 -4158*n^6 + 29421*n^5 -148743*n^4 +
 *            530796*n^3 -1276992*n^2 +1866384*n -1255608 (for n ≥ 7; for 0 ≤
 *            n ≤ 6 the given initial values are used)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 141, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        1ULL,          /* a(0) */
        10ULL,         /* a(1) */
        364ULL,        /* a(2) */
        4664ULL,       /* a(3) */
        40296ULL,      /* a(4) */
        253072ULL,     /* a(5) */
        1249768ULL      /* a(6) */
    };

    if (n < 7) return init[n];

    /* Evaluate the 9‑th degree polynomial using Horner's rule.
       The calculation is performed with signed 128‑bit integers
       to avoid overflow for the range of n expected in the tests. */
    signed __int128 nn = (signed __int128)n;
    signed __int128 p  = 1;            /* coefficient of n^9 */
    p = p * nn + (-27);
    p = p * nn +  414;
    p = p * nn + (-4158);
    p = p * nn +  29421;
    p = p * nn + (-148743);
    p = p * nn +  530796;
    p = p * nn + (-1276992);
    p = p * nn +  1866384;
    p = p * nn + (-1255608);

    return (seq_t)p;
}
