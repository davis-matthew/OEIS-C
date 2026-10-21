/*
 * A211034  Number of 2 X 2 matrices having all elements in {0,1,...,n} and
 *          determinant = 1 (mod 3).
 * https://oeis.org/A211034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n ≥ 0 let r = floor(n/3)+1, s = floor((n-1)/3)+1, t =
 *            floor((n-2)/3)+1. Then a(n) = (8*n^4 + 28*n^3 + 33*n^2 + 16*n -
 *            4)/27 if n ≡ 1 (mod 3) 4*n^2*(2*n^2 + 6*n + 3)/27 if n ≡ 0 (mod
 *            3) 8*(n+1)^4/27 if n ≡ 2 (mod 3) (The formula is exact for all n
 *            ≥ 0.)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 345, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* cast index to value type */
    idx_t m = n % 3;             /* n modulo 3 */

    switch (m) {
        case 0: {
            seq_t x2 = x * x;                      /* n^2   */
            seq_t inner = (seq_t)2 * x2            /* 2*n^2 */
                       + (seq_t)6 * x              /* +6*n  */
                       + (seq_t)3;                 /* +3    */
            seq_t term = (seq_t)4 * x2 * inner;    /* 4*n^2*inner */
            return term / (seq_t)27;              /* divide by 27 */
        }
        case 1: {
            seq_t term = (seq_t)8 * (x * x * x * x)   /* 8*n^4 */
                      + (seq_t)28 * (x * x * x)      /* 28*n^3 */
                      + (seq_t)33 * (x * x)        /* 33*n^2 */
                      + (seq_t)16 * x              /* 16*n   */
                      - (seq_t)4;                 /* -4     */
            return term / (seq_t)27;              /* divide by 27 */
        }
        case 2: {
            seq_t y = x + (seq_t)1;                /* n+1   */
            seq_t term = (seq_t)8 * (y * y * y * y);/* 8*(n+1)^4 */
            return term / (seq_t)27;              /* divide by 27 */
        }
    }
    /* unreachable, but return 0 for safety */
    return (seq_t)0;
}
