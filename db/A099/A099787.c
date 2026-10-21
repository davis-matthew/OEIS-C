/*
 * A099787  a(n) = Sum_{k=0..floor(n/4)} C(n-k,3*k) * 2^k * 3^(n-4*k).
 * https://oeis.org/A099787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 9*a(n-1) - 27*a(n-2) + 27*a(n-3) + 2*a(n-4)
    a(0)=1, a(1)=3, a(2)=9, a(3)=27 */

#include <stdlib.h>

static seq_t *memo = NULL;      /* memoized terms */
static size_t memo_len = 0;     /* number of stored terms */

seq_t a(idx_t n)
{
    /* enlarge memoization array if needed */
    if ((size_t)n >= memo_len) {
        size_t old_len = memo_len;
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (!tmp)     /* out‑of‑memory – return 0 as sentinel */
            return 0;
        memo = tmp;
        memo_len = new_len;

        /* compute all missing terms */
        for (idx_t i = (idx_t)old_len; i <= n; ++i) {
            if (i == 0)
                memo[i] = 1;
            else if (i == 1)
                memo[i] = 3;
            else if (i == 2)
                memo[i] = 9;
            else if (i == 3)
                memo[i] = 27;
            else {
                seq_t sum = 0;
                sum += memo[i - 1] * 9;
                sum -= memo[i - 2] * 27;
                sum += memo[i - 3] * 27;
                sum += memo[i - 4] * 2;
                memo[i] = sum;
            }
        }
    }
    return memo[(size_t)n];
}
