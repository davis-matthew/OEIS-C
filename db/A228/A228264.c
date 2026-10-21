/*
 * A228264  Number of third differences of arrays of length 8 of numbers in
 *          0..n.
 * https://oeis.org/A228264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 672*n^5 - 1152*n^4 - 3340*n^3 + 8908*n^2 + 4108*n - 14279
 *            for n>13; precomputed values for n<=13
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 13 terms (n = 1..13) */
    static const seq_t table[13] = {
        (seq_t)255,        /* a(1) */
        (seq_t)6305,       /* a(2) */
        (seq_t)58911,      /* a(3) */
        (seq_t)315601,     /* a(4) */
        (seq_t)1161855,    /* a(5) */
        (seq_t)3298681,    /* a(6) */
        (seq_t)7795501,    /* a(7) */
        (seq_t)16171769,   /* a(8) */
        (seq_t)30424997,   /* a(9) */
        (seq_t)53235009,   /* a(10) */
        (seq_t)87985685,   /* a(11) */
        (seq_t)138831929,  /* a(12) */
        (seq_t)210793509   /* a(13) */
    };

    if (n == 0) return 0;             /* undefined for n<1, return 0 */
    if (n <= 13) return table[n - 1];

    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t n4   = n3 * nn;
    seq_t n5   = n4 * nn;

    seq_t res  = (seq_t)672  * n5;
    res -= (seq_t)1152 * n4;
    res -= (seq_t)3340 * n3;
    res += (seq_t)8908 * n2;
    res += (seq_t)4108 * nn;
    res -= (seq_t)14279;

    return res;
}
