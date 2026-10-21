/*
 * A081742  a(1)=1; then if n is a multiple of 3, a(n) = a(n/3) + 1; if n is
 *          not a multiple of 3 but even, a(n) = a(n/2) + 1; a(n) = a(n-1) + 1
 *          otherwise.
 * https://oeis.org/A081742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n) = a(n/3)+1 if n%3==0; a(n) = a(n/2)+1 if n%2==0;
 *            a(n) = a(n-1)+1 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    /* we treat n==0 as undefined; return 0 in that case */
    if (n == 0) return 0;

    /* enlarge table if necessary */
    if (n >= memo_size) {
        idx_t new_size = n + 1;                /* indices 0..n inclusive */
        seq_t *new_table = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!new_table) return 0;              /* allocation failure */
        memo = new_table;
        for (idx_t i = memo_size; i < new_size; ++i) memo[i] = 0;
        memo_size = new_size;
    }

    /* base value */
    if (n == 1) {
        memo[1] = 1;
        return 1;
    }

    /* cached value? */
    if (memo[n] != 0) return memo[n];

    /* compute recursively */
    seq_t result;
    if ((n % 3) == 0) {
        result = a(n / 3) + 1;
    } else if ((n % 2) == 0) {
        result = a(n / 2) + 1;
    } else {
        result = a(n - 1) + 1;
    }

    /* store and return */
    memo[n] = result;
    return result;
}
