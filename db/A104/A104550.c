/*
 * A104550  Number of horizontal segments in all Schroeder paths of length 2n
 *          (a horizontal segment is a maximal string of horizontal steps).
 * https://oeis.org/A104550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((7n-6)*a(n-1) - (7n-22)*a(n-2) + (n-4)*a(n-3)) / n
 *            initial values: a(1)=1, a(2)=4, a(3)=20 (n,a(n)) pairs are
 *            1-based
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *memo = NULL;        /* memoized terms */
static idx_t  memo_cap = 0;       /* allocated size of memo (in elements) */
static idx_t  memo_max = 0;       /* highest index for which a(i) is stored */

/* Ensure memo has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= memo_cap)
        return;
    size_t new_cap = memo_cap ? memo_cap : 8;
    while (new_cap <= n)
        new_cap <<= 1;                 /* grow by factors of 2 */
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory – abort. */
        exit(1);
    }
    memo = tmp;
    memo_cap = new_cap;
}

/* Initialise the base cases a(1)=1, a(2)=4, a(3)=20. */
static void init_base(void)
{
    if (memo_max >= 3)
        return;
    ensure_capacity(3);   /* indices 0..3 */
    memo[0] = 0;
    memo[1] = (seq_t)1;
    memo[2] = (seq_t)4;
    memo[3] = (seq_t)20;
    memo_max = 3;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but safe */
    init_base();
    if (n <= memo_max)
        return memo[n];

    ensure_capacity(n);
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        seq_t rhs;
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 v1 = (unsigned __int128)memo[i-1] * (unsigned __int128)(7*i - 6);
            unsigned __int128 v2 = (unsigned __int128)memo[i-2] * (unsigned __int128)(7*i - 22);
            unsigned __int128 v3 = (unsigned __int128)memo[i-3] * (unsigned __int128)(i - 4);
            unsigned __int128 numer = v1 - v2 + v3;
            rhs = (seq_t)(numer / (unsigned __int128)i);
        } else {
            seq_t v1 = memo[i-1] * (seq_t)(7*i - 6);
            seq_t v2 = memo[i-2] * (seq_t)(7*i - 22);
            seq_t v3 = memo[i-3] * (seq_t)(i - 4);
            seq_t numer = v1 - v2 + v3;
            rhs = numer / (seq_t)i;
        }
        memo[i] = rhs;
    }
    memo_max = n;
    return memo[n];
}
