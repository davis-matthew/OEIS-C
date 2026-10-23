/*
 * A102122  Iccanobirt numbers (12 of 15): a(n) = R(R(a(n-1)) + a(n-2) +
 *          a(n-3)), where R is the digit reversal function A004086.
 * https://oeis.org/A102122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R( R(a(n-1)) + a(n-2) + a(n-3) ) (a(0)=0, a(1)=0, a(2)=1)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/rev.h"          /* provides seq_t rev(seq_t) */

static seq_t *memo = NULL;          /* dynamic array of computed values */
static idx_t  memo_len = 0;         /* current allocated length */
static idx_t  memo_max = (idx_t)-1; /* highest index computed */

/* Ensure the memo array can hold indices 0 … need */
static void ensure_cap(idx_t need)
{
    if (need < memo_len) return;                /* already large enough */

    idx_t newlen = memo_len ? memo_len << 1 : 8;
    while (newlen <= need) newlen <<= 1;        /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(memo, newlen * sizeof(seq_t));
    if (!tmp) abort();                          /* out of memory */
    memo = tmp;
    memo_len = newlen;
}

/* Initialise the memo table with the first three values */
static void init_memo(void)
{
    if (memo) return;                           /* already initialised */

    memo_len = 3;
    memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
    if (!memo) abort();                          /* out of memory */

    memo[0] = (seq_t)0;
    memo[1] = (seq_t)0;
    memo[2] = (seq_t)1;

    memo_max = 2;
}

/* Main sequence function as required */
seq_t a(idx_t n)
{
    init_memo();                    /* guarantees memo[0..2] exist */

    if (n <= memo_max)             /* already computed */
        return memo[n];

    /* compute up to n, extending memo as required */
    ensure_cap(n);

    for (idx_t i = memo_max + 1; i <= n; ++i) {
        seq_t revprev = rev(memo[i - 1]);          /* R(a(i-1))          */
        seq_t sum     = revprev + memo[i - 2] + memo[i - 3];
        memo[i]       = rev(sum);                  /* outer R            */
    }
    memo_max = n;
    return memo[n];
}
