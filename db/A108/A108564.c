/*
 * A108564  a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 4, for n>3: a(n+1)
 *          = SORT[a(n) + a(n-1) + a(n-2) + a(n-3)], where SORT places digits
 *          in ascending order and deletes 0's.
 * https://oeis.org/A108564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=4, for n>3 a(n)=SORT[
 *            a(n-1)+a(n-2)+a(n-3)+a(n-4) ] (SORT removes 0's and sorts the
 *            decimal digits in ascending order)
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sort_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sort_digits.h"

static seq_t *memo = NULL;          /* cached terms */
static size_t memo_cap = 0;         /* capacity of memo */
static size_t memo_len = 0;         /* number of terms stored */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Initialise the cache on first use.  The first five terms are
       known explicitly. */
    if (memo_cap == 0) {
        memo_cap = 5;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0;          /* fallback if allocation fails */
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 2;
        memo[4] = 4;
        memo_len = 5;
    }

    /* Return a cached value if already known */
    if (n < memo_len)
        return memo[n];

    /* Extend the cache up to n */
    for (idx_t i = (idx_t)memo_len; i <= n; ++i) {
        /* Ensure we have enough space */
        if (memo_len >= memo_cap) {
            size_t new_cap = memo_cap * 2;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) {                    /* allocation failed: compute
                                               iteratively and return. */
                seq_t a = memo[memo_len - 4];
                seq_t b = memo[memo_len - 3];
                seq_t c = memo[memo_len - 2];
                seq_t d = memo[memo_len - 1];
                for (idx_t k = (idx_t)memo_len; k <= n; ++k) {
                    seq_t sum = a + b + c + d;
                    seq_t next = sort_digits(sum);
                    a = b; b = c; c = d; d = next;
                }
                return d;
            }
            memo = tmp;
            memo_cap = new_cap;
        }

        /* Compute the next term */
        seq_t sum = memo[(size_t)(i - 1)] + memo[(size_t)(i - 2)] +
                    memo[(size_t)(i - 3)] + memo[(size_t)(i - 4)];
        memo[(size_t)i] = sort_digits(sum);
        memo_len++;
    }

    return memo[(size_t)n];
}
