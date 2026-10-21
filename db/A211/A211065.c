/*
 * A211065  Number of 2 X 2 matrices having all terms in {1,...,n} and odd
 *          determinant.
 * https://oeis.org/A211065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n + 1 - (-1)^n)^2 * (6*n + 1 - (-1)^n) * (2*n - 1 +
 *            (-1)^n) / 128
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 325, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t1, t2, t3, numer;

    if ((n & 1) == 0) {            /* n even → (-1)^n =  1 */
        t1 = (seq_t)2 * tn;         /* 2n + 1 - 1  = 2n */
        t2 = (seq_t)6 * tn;         /* 6n + 1 - 1  = 6n */
        t3 = (seq_t)2 * tn;         /* 2n - 1 + 1  = 2n */
    } else {                        /* n odd  → (-1)^n = -1 */
        t1 = (seq_t)2 * tn + (seq_t)2; /* 2n + 1 - (-1) = 2n + 2 */
        t2 = (seq_t)6 * tn + (seq_t)2; /* 6n + 1 - (-1) = 6n + 2 */
        t3 = (seq_t)2 * tn - (seq_t)2; /* 2n - 1 + (-1) = 2n - 2 */
    }

    numer = t1;
    numer = numer * t1;  /* t1^2 */
    numer = numer * t2;
    numer = numer * t3;

    return numer / (seq_t)128;
}
