/*
 * A100069  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*4^(n-2*k).
 * https://oeis.org/A100069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k)*4^(n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (n ≥ 4): 8*n*a(n) = 2*(19*n-4)*a(n-1) + (15*n+2)*a(n-2)
 *                    - 8*(19*n-23)*a(n-3) + 68*(n-3)*a(n-4)
 * Initial values: a(0)=1, a(1)=4, a(2)=18, a(3)=76
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoization array */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    /* On first call, initialize base values up to 3 */
    if (memo_size == 0) {
        memo_size = 4;
        memo = (seq_t *)malloc((size_t)memo_size * sizeof(seq_t));
        if (!memo) return 0; /* allocation failure, return 0 as fallback */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)4;
        memo[2] = (seq_t)18;
        memo[3] = (seq_t)76;
    }

    /* If value already computed, return it */
    if (n < memo_size)
        return memo[n];

    /* Expand memo to accommodate up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure */
    memo = tmp;

    /* Compute values from memo_size up to n */
    for (idx_t i = memo_size; i <= n; ++i) {
        if (i <= 3) {
            /* Base cases already set */
            continue;
        }
        seq_t term1 = (seq_t)2 * ((seq_t)19 * (seq_t)i - (seq_t)4) * memo[i - 1];
        seq_t term2 = ((seq_t)15 * (seq_t)i + (seq_t)2) * memo[i - 2];
        seq_t term3 = (seq_t)8 * ((seq_t)19 * (seq_t)i - (seq_t)23) * memo[i - 3];
        seq_t term4 = (seq_t)68 * ((seq_t)i - (seq_t)3) * memo[i - 4];

        seq_t numerator = term1 + term2 - term3 + term4;
        seq_t denom = (seq_t)8 * (seq_t)i;
        memo[i] = numerator / denom;  /* exact division */
    }

    memo_size = n + 1;
    return memo[n];
}
