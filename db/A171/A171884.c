/*
 * A171884  Lexicographically earliest injective nonnegative sequence a(n)
 *          satisfying |a(n+1) - a(n)| = n for all n.
 * https://oeis.org/A171884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1; for n≥3 let k be the unique integer with 2·3^k+1
 *            ≤ n ≤ 6·3^k (i.e. 6*3^k ≥ n). Let power=3^k. If n is even: a(n)
 *            = 7·power – 2 + (n – 2·power)/2. If n is odd : a(n) = 5·power –
 *            1 – ((n + 1 – 2·power)/2)
 * Range:     u8 n <= 162, u16 n <= 9462, u32 n <= 9462, u64 n <= 9462,
 *            u128 n <= 9462, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    idx_t power = 1;               /* power = 3^k */
    while ((idx_t)6 * power < n)   /* find k so that 6*power ≥ n   */
        power *= 3;

    seq_t sp = (seq_t)power;        /* cast power to seq_t */

    if ((n & 1) == 0) {            /* n is even   */
        idx_t r = (n - (idx_t)2 * power) / 2;
        return (seq_t)7 * sp - (seq_t)2 + (seq_t)r;
    } else {                       /* n is odd    */
        idx_t r = ((n + 1) - (idx_t)2 * power) / 2;
        return (seq_t)5 * sp - (seq_t)1 - (seq_t)r;
    }
}
