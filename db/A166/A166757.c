/*
 * A166757  Number of nX2 1..4 arrays containing at least one of each value,
 *          and all equal values connected.
 * https://oeis.org/A166757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4/15)*(n-1)*(8*n^5 -28*n^4 +97*n^3 -218*n^2 +234*n -90)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(1) and a(0) are 0 */

    seq_t nn = (seq_t)n;          /* cast the index once */
    seq_t n1 = nn - (seq_t)1;     /* (n-1) */

    /* Compute n^2 … n^5 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;

    /* Polynomial Q(n) = 8*n^5 -28*n^4 +97*n^3 -218*n^2 +234*n -90 */
    seq_t Q = (seq_t)8 * n5;
    Q -= (seq_t)28 * n4;
    Q += (seq_t)97 * n3;
    Q -= (seq_t)218 * n2;
    Q += (seq_t)234 * nn;
    Q -= (seq_t)90;

    /* The final result is (4*(n-1)*Q)/15.
       Use a wider integer to avoid intermediate overflow (required for the
       64‑bit seq_t build). */
#if defined(__SIZEOF_INT128__)
    __int128 prod = (__int128)n1 * (__int128)Q;   /* (n-1)*Q */
    prod = prod * 4;                              /* *4 */
    prod /= 15;                                   /* /15 */
    return (seq_t)prod;
#else
    /* Fallback for systems without 128‑bit support.
       Perform the division early to keep intermediate values small. */
    /* Reduce 15 across the factors 4, n1 and Q. */
    int d = 1;
    for (int i = 2; i <= 15; ++i)
        if (15 % i == 0 && (i == 4 || n1 % i == 0 || Q % i == 0))
            d = i;                 /* largest divisor that divides one factor */

    seq_t part = n1 / ((seq_t)d);          /* divide that factor by d */
    seq_t rem = 15 / d;                   /* remaining divisor */

    /* Now (4*part*Q)/rem is integer.  Compute in 128‑bit if available. */
#if defined(__INT128__)
    __int128 prod = (__int128)part * (__int128)Q * 4;
    prod /= rem;
    return (seq_t)prod;
#else
    /* All values are small enough for 64-bit; no overflow occurs. */
    seq_t prod = part * Q;
    prod *= 4;
    prod /= rem;
    return prod;
#endif
#endif
}
