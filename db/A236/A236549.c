/*
 * A236549  The number of matchings in the n-flower graph.
 * https://oeis.org/A236549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) =
 *            8*a(n-1)+31*a(n-2)-68*a(n-3)-152*a(n-4)+128*a(n-5)+31*a(n-6)-20*a(n-7)-a(n-8)
 *            initial values: a(1)=8, a(2)=126, a(3)=1052, a(4)=11170,
 *            a(5)=112828, a(6)=1159416, a(7)=11869768, a(8)=121668290
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *memo = NULL;       /* cache of computed values   */
static size_t memo_cap = 0;      /* allocated elements         */
static size_t memo_last = 0;     /* highest index that is valid */

static void ensure_memo_size(size_t n)
/* enlarge the memo array so that it can hold indices up to n (inclusive) */
{
    size_t need = n;
    if (need < 8) need = 8;                  /* needed for the base terms */
    if (memo_cap > need) return;             /* already sufficiently large */

    size_t new_cap = memo_cap ? memo_cap : 1;
    while (new_cap <= need) new_cap <<= 1;   /* grow to the next power of two */

    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    memo = tmp;
    memo_cap = new_cap;

    /* initialize base terms once */
    if (memo_last == 0) {
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)8;
        memo[2] = (seq_t)126;
        memo[3] = (seq_t)1052;
        memo[4] = (seq_t)11170;
        memo[5] = (seq_t)112828;
        memo[6] = (seq_t)1159416;
        memo[7] = (seq_t)11869768;
        memo[8] = (seq_t)121668290;
        memo_last = 8;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* out of range -> 0 */

    ensure_memo_size((size_t)n);

    if ((size_t)n <= memo_last) return memo[(size_t)n];

    for (size_t i = memo_last + 1; i <= (size_t)n; ++i) {
        memo[i] =
            (seq_t)8   * memo[(size_t)(i-1)] +
            (seq_t)31  * memo[(size_t)(i-2)] -
            (seq_t)68  * memo[(size_t)(i-3)] -
            (seq_t)152 * memo[(size_t)(i-4)] +
            (seq_t)128 * memo[(size_t)(i-5)] +
            (seq_t)31  * memo[(size_t)(i-6)] -
            (seq_t)20  * memo[(size_t)(i-7)] -
            memo[(size_t)(i-8)];
    }
    memo_last = (size_t)n;
    return memo[(size_t)n];
}
