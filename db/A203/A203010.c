/*
 * A203010  (n-1)-st elementary symmetric function of first n Lucas numbers,
 *          starting with L(1)=1.
 * https://oeis.org/A203010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=1..n} L(i) * sum_{i=1..n} 1/L(i) where L(1)=1,
 *            L(2)=3 and L(k)=L(k-1)+L(k-2) for k>=3. This is the (n-1)-st
 *            elementary symmetric function of the first n Lucas numbers
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* below offset, not used */
    if (n == 1) return 1;         /* a(1) = 1 */

    seq_t prod   = 1;             /* product of Lucas numbers up to current */
    seq_t result = 1;             /* a value for previous n (a(n-1)) */

    /* Initial Lucas numbers L(1)=1, L(2)=3 */
    seq_t lprev = 1;              /* L(k-1) */
    seq_t lcurr = 3;              /* L(k)   */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t Lk     = lcurr;      /* current Lucas number */
        seq_t prod_prev = prod;    /* product up to L(k-1) */

        prod   = prod * Lk;        /* product up to L(k)   */
        result = Lk * result + prod_prev; /* a(k) = Lk*a(k-1)+P_{k-1} */

        /* advance Lucas numbers for next iteration */
        seq_t next = lcurr + lprev;
        lprev = lcurr;
        lcurr = next;
    }
    return result;
}
