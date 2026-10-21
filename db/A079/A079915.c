/*
 * A079915  Solution to the Dancing School Problem with 10 girls and n+10
 *          boys: f(10,n).
 * https://oeis.org/A079915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^10 - 35*n^9 + 675*n^8 - 8610*n^7 + 78435*n^6 -
 *            523467*n^5 + 2562525*n^4 - 9008160*n^3 + 21623220*n^2 -
 *            31840760*n + 21750840 (for n ≥ 8) For 0 ≤ n < 8 the terms are
 *            taken from the definition
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 8 values from the OEIS entry */
    static const seq_t pre[8] = {
        1ULL,            /* a(0) */
        11ULL,           /* a(1) */
        596ULL,          /* a(2) */
        9627ULL,         /* a(3) */
        103129ULL,       /* a(4) */
        780902ULL,       /* a(5) */
        4557284ULL,      /* a(6) */
        21670160ULL      /* a(7) */
    };

    if (n < 8)
        return pre[n];

    /* compute powers of n up to n^10 */
    seq_t x   = (seq_t)n;      /* convert to seq_t once */
    seq_t pow[11];
    pow[0] = 1;
    for (int i = 1; i <= 10; ++i)
        pow[i] = pow[i - 1] * x;

    /* accumulate positive and negative parts separately to avoid underflow on unsigned types */
    seq_t pos = 0;
    pos += pow[10];                     /* +1 * n^10           */
    pos += 675      * pow[8];            /* +675 * n^8          */
    pos += 78435    * pow[6];            /* +78435 * n^6       */
    pos += 2562525  * pow[4];            /* +2562525 * n^4      */
    pos += 21623220 * pow[2];            /* +21623220 * n^2     */
    pos += 21750840;                     /* +21750840           */

    seq_t neg = 0;
    neg += 35       * pow[9];            /* +35 * n^9           */
    neg += 8610     * pow[7];            /* +8610 * n^7         */
    neg += 523467   * pow[5];            /* +523467 * n^5       */
    neg += 9008160  * pow[3];            /* +9008160 * n^3      */
    neg += 31840760 * pow[1];            /* +31840760 * n       */

    return pos - neg;
}
