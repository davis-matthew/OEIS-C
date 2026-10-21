/*
 * A212507  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x and
 *          y<=2z.
 * https://oeis.org/A212507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(9*n^3+6*n^2+1)-(2*n-1)*(-1)^n-1)/32
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t expr = 9 * n3 + 6 * n2 + 1;          /* 9*n^3 + 6*n^2 + 1 */
    seq_t base = 2 * n_seq * expr;             /* 2*n*(9*n^3+6*n^2+1) */
    seq_t term = 2 * n_seq - 1;                /* 2*n-1 */

    seq_t num;
    if ((n & 1) == 0) {                        /* n even: subtract (2*n-1) */
        num = base;
        num -= term;
        num -= 1;
    } else {                                   /* n odd: add (2*n-1)  */
        num = base;
        num += term;
        num -= 1;
    }

    return num / 32;                           /* division by 32 */
}
