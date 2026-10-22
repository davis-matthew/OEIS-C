/*
 * A003708  Expansion of tan(log(1+x)).
 * https://oeis.org/A003708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   (1+x)*A'(x) = 1 + A(x)^2 for the EGF A(x) = tan(log(1+x)),
 *            giving a(n) = Sum_{k=0..n-1} binomial(n-1,k)*a(k)*a(n-1-k) -
 *            (n-1)*a(n-1) for n >= 2, a(0) = 0, a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static seq_t *row = NULL;      /* row[k] = binomial(computed-1, k) */
    static idx_t cap = 0;
    static idx_t computed = 0;

    if (cap <= n) {
        idx_t newcap = (n + 1) * 2;
        seq_t *nm = (seq_t *)malloc(newcap * sizeof(seq_t));
        seq_t *nr = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (idx_t i = 0; i < computed; i++) {
            nm[i] = memo[i];
            nr[i] = row[i];
        }
        free(memo);
        free(row);
        memo = nm;
        row = nr;
        cap = newcap;
    }

    if (computed == 0) {
        memo[0] = 0;
        memo[1] = 1;
        row[0] = 1;           /* binomial(1,0) */
        row[1] = 1;           /* binomial(1,1) */
        computed = 2;
    }

    while (computed <= n) {
        idx_t i = computed;
        /* row currently holds binomial(i-2, .); advance to binomial(i-1, .) */
        row[i - 1] = 1;
        for (idx_t k = i - 2; k >= 1; k--)
            row[k] = row[k] + row[k - 1];
        seq_t sum = 0;
        for (idx_t k = 1; k + 1 < i; k++)
            sum += row[k] * memo[k] * memo[i - 1 - k];
        memo[i] = sum - (seq_t)(i - 1) * memo[i - 1];
        computed++;
    }

    return memo[n];
}
