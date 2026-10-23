/*
 * A186359  Number of permutations of {1,2,...,n} having no up-down cycles. A
 *          cycle (b(1), b(2), ...) is said to be up-down if, when written
 *          with its smallest element in the first position, it satisfies
 *          b(1)<b(2)>b(3)<... .
 * https://oeis.org/A186359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, for n>=2: if n is even: a(n) = n*a(n-1) if n is
 *            odd: a(n) = n*a(n-1) + (-1)^{(n-1)/2+1}
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t prev1 = (seq_t)0;   /* a(1) */
    seq_t cur;                /* a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t prod = (seq_t)i * prev1;   /* n * a(n-1) */
        if (i & 1) {                     /* n odd */
            idx_t k = (i - 1) / 2;
            if (k & 1)       /* k odd => +1 */
                cur = prod + 1;
            else             /* k even => -1 */
                cur = prod - 1;
        } else {                       /* n even */
            cur = prod;
        }
        prev1 = cur;
    }
    return prev1;
}
