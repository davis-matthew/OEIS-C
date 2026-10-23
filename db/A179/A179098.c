/*
 * A179098  Rectified 7-simplex number: the coefficient of x^(2n-2) in
 *          (1+x+x^2+...+x^(n-1))^8.
 * https://oeis.org/A179098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(90+77*n+140*n^3+210*n^4+98*n^5+15*n^6)/630
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 15, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t ns = (seq_t)n;           /* cast once to seq_t */
    seq_t n2 = ns * ns;            /* n^2 */
    seq_t n3 = n2 * ns;            /* n^3 */
    seq_t n4 = n3 * ns;            /* n^4 */
    seq_t n5 = n4 * ns;            /* n^5 */
    seq_t n6 = n5 * ns;            /* n^6 */

    seq_t p = (seq_t)90;           /* 90 + 77 n + 140 n^3 + 210 n^4 + 98 n^5 + 15 n^6 */
    p += (seq_t)77  * ns;
    p += (seq_t)140 * n3;
    p += (seq_t)210 * n4;
    p += (seq_t)98  * n5;
    p += (seq_t)15  * n6;

    seq_t numerator = ns * p;      /* n * p */
    return numerator / (seq_t)630; /* exact division */
}
