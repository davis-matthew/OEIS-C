/*
 * A125196  Number of magic labelings of the Petersen graph with magic sum n.
 * https://oeis.org/A125196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/32)*(29*C(n+5,5) + 21*C(n+4,5) + 126*C(n+3,5) * -
 *            34*C(n+2,5) + 21*C(n+1,5) - 3*C(n,5) * + 3*(-1)^n) (OEIS
 *            A125196)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 107, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom5(seq_t k)
{
    /* Compute C(k,5).  Assume k is unsigned (seq_t). */
    if (k < (seq_t)5) return (seq_t)0;
    seq_t res = 1;
    res *= k;        /* k          */
    res *= (k - (seq_t)1);
    res *= (k - (seq_t)2);
    res *= (k - (seq_t)3);
    res *= (k - (seq_t)4);
    res /= (seq_t)120;  /* 5! = 120 */
    return res;
}

seq_t a(idx_t n)
{
    /* binomial terms */
    seq_t c0 = binom5((seq_t)(n + 0)); /* C(n,5)  */
    seq_t c1 = binom5((seq_t)(n + 1)); /* C(n+1,5) */
    seq_t c2 = binom5((seq_t)(n + 2)); /* C(n+2,5) */
    seq_t c3 = binom5((seq_t)(n + 3)); /* C(n+3,5) */
    seq_t c4 = binom5((seq_t)(n + 4)); /* C(n+4,5) */
    seq_t c5 = binom5((seq_t)(n + 5)); /* C(n+5,5) */

    /* positive and negative contributions */
    seq_t pos = 29 * c5 + 21 * c4 + 126 * c3 + 21 * c1;
    seq_t neg = 34 * c2 + 3 * c0;

    /* add the parity term 3*(-1)^n */
    if ((n & 1) == 0)  /* n even: add 3 */
        pos += 3;
    else               /* n odd: subtract 3 */
        neg += 3;

    /* numerator is positive by construction */
    seq_t num = pos - neg;
    return num / 32;
}
