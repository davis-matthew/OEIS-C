/*
 * A086881  a(n) = (2*n)!*Sum[Sum[1/(i+j),{i,1,n}],{j,1,n}].
 * https://oeis.org/A086881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! * Sum_{i=1}^{n} Sum_{j=1}^{n} 1/(i+j) The sum can
 *            be rewritten as a(n) = (2n)! * sum_{k=2}^{2n} count(k)/k where
 *            count(k) = number of pairs (i,j) with 1<=i,j<=n and i+j=k. For k
 *            in [2, n+1] count(k) = k-1 For k in [n+2, 2n] count(k) = 2n-k+1
 *            Since k <= 2n, (2n)! is divisible by k, so...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    idx_t two_n = n * 2;
    seq_t fact = 1;
    for (idx_t i = 2; i <= two_n; ++i)
        fact *= (seq_t)i;          /* fact = (2n)! */

    seq_t sum = 0;

    /* k from 2 to n+1 */
    for (idx_t k = 2; k <= n + 1; ++k) {
        seq_t count = (seq_t)(k - 1);
        seq_t term  = fact / (seq_t)k;
        sum += count * term;
    }

    /* k from n+2 to 2n */
    for (idx_t k = n + 2; k <= two_n; ++k) {
        seq_t count = (seq_t)(two_n - k + 1);
        seq_t term  = fact / (seq_t)k;
        sum += count * term;
    }

    return sum;
}
