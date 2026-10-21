/*
 * A223442  4-level binary fanout graph coloring a rectangular array: number
 *          of n X 1 0..14 arrays where 0..14 label nodes of a graph with
 *          edges 0,1 1,3 3,5 3,6 1,4 4,7 4,8 0,2 2,9 9,11 9,12 2,10 10,13
 *          10,14 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) - 4*a(n-4), a(1)=15, a(2)=28, a(3)=66, a(4)=144
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;          /* memoization array, index starts at 1 */
static size_t memo_cap = 0;         /* allocated capacity of memo */
static size_t memo_max = 0;         /* largest n computed so far */

/* initialise memo[1..4] */
static void init_memo(void)
{
    if (memo != NULL)
        return;

    memo_cap = 16;
    memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    memo[1] = (seq_t)15;
    memo[2] = (seq_t)28;
    memo[3] = (seq_t)66;
    memo[4] = (seq_t)144;
    memo_max = 4;
}

/* ensure that values up to n are computed */
static void ensure_n(idx_t n)
{
    size_t nn = (size_t)n;
    init_memo();

    if (nn <= memo_max)
        return;

    /* grow capacity if necessary */
    if (nn + 1 > memo_cap) {
        size_t new_cap = memo_cap;
        while (nn + 1 > new_cap)
            new_cap *= 2;
        memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo_cap = new_cap;
    }

    /* extend memo array using the recurrence */
    for (size_t i = memo_max + 1; i <= nn; ++i) {
        seq_t v2 = memo[i - 2];
        seq_t v4 = memo[i - 4];
        memo[i] = (seq_t)6 * v2 - (seq_t)4 * v4;
    }

    memo_max = nn;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 as sentinel */

    ensure_n(n);
    return memo[(size_t)n];
}
