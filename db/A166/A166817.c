/*
 * A166817  Number of n X 2 1..6 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          increasing order.
 * https://oeis.org/A166817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A166817: Number of n×2 arrays over {1..6} containing each value at least once,
    with all equal values connected, rows nondecreasing and columns increasing.
    Formula (OEIS): 
        a(n) = (16/10!)*n*(n-1)*(n-2)*(361620 - 1844397*n + 175952*n^2
                    + 173779*n^3 + 23435*n^4 + 1477*n^5 + 53*n^6 + n^7).
    Offset: 1
 */
seq_t a(idx_t n)
{
    /* For n < 3 the product n*(n-1)*(n-2) is zero, so a(n)=0. */
    if (n < 3)
        return 0;

    seq_t sn  = (seq_t)n;          /* n */
    seq_t n1  = sn - 1;            /* n-1 */
    seq_t n2  = sn - 2;            /* n-2 */

    /* Compute powers of n up to n^7 */
    seq_t n2_ = sn * sn;            /* n^2 */
    seq_t n3_ = n2_ * sn;           /* n^3 */
    seq_t n4_ = n3_ * sn;           /* n^4 */
    seq_t n5_ = n4_ * sn;           /* n^5 */
    seq_t n6_ = n5_ * sn;           /* n^6 */
    seq_t n7_ = n6_ * sn;           /* n^7 */

    /* Evaluate the polynomial part: 
       P(n) = 361620 - 1844397*n + 175952*n^2 + 173779*n^3
              + 23435*n^4 + 1477*n^5 + 53*n^6 + n^7
     */
    seq_t poly = 361620;
    poly -= 1844397 * sn;
    poly += 175952 * n2_;
    poly += 173779 * n3_;
    poly += 23435  * n4_;
    poly += 1477   * n5_;
    poly += 53     * n6_;
    poly += n7_;

    /* Compute numerator: n*(n-1)*(n-2)*poly   */
    seq_t num = sn * n1 * n2 * poly;

    /* Denominator = 226800 ( = 16/10! ) */
    seq_t res = num / 226800;

    return res;
}
