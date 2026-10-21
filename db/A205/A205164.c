/*
 * A205164  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          unequal diagonal elements or unequal antidiagonal elements, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A205164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 159 a(2) = 8479 a(3) = 474853 a(4) = 26702419 a(n) =
 *            62*a(n-1) - 329*a(n-2) + 324*a(n-3) - 84*a(n-4) (n ≥ 5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;          /* 1‑indexed memoisation array */
static idx_t  memo_cap = 0;         /* maximum allocated index (inclusive)   */
static idx_t  memo_len = 0;         /* largest index for which a value is stored */

/* Ensure that the memo array can hold indices up to n (inclusive) and
   initialise the first four terms when required.                           */
static void ensure_memo(idx_t n)
{
    if (n + 1 <= memo_cap) return;

    idx_t old_cap  = memo_cap;
    idx_t new_cap  = memo_cap ? memo_cap : 8;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(memo, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) return;          /* allocation failure – caller will interpret as 0 */
    memo = tmp;
    memo_cap = new_cap;

    /* zero‑initialise newly allocated elements */
    for (idx_t i = old_cap + 1; i <= new_cap; ++i) memo[i] = 0;

    /* initialise the known first four terms if they are not yet set */
    if (memo[1] == 0) {
        memo[1] = 159;
        memo[2] = 8479;
        memo[3] = 474853;
        memo[4] = 26702419;
    }

    /* record that values up to index 4 are now present */
    if (memo_len == 0) memo_len = 4;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    ensure_memo(n);

    if (n <= memo_len) return memo[n];

    /* compute missing values up to n */
    for (idx_t i = memo_len + 1; i <= n; ++i) {
        seq_t term = 62 * memo[i - 1];
        term -= 329 * memo[i - 2];
        term += 324 * memo[i - 3];
        term -= 84 * memo[i - 4];
        memo[i] = term;
    }
    memo_len = n;
    return memo[n];
}
