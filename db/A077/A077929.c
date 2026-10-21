/*
 * A077929  Expansion of (1-x)^(-1)/(1+2*x+x^2-x^3).
 * https://oeis.org/A077929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-1, a(2)=2, a(3)=-1, for n>=4 a(n) = -(a(n-1)) +
 *            a(n-2) + 2*a(n-3) - a(n-4)
 * Range:     i8 n <= 15, i16 n <= 27, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Generating function: 1/((1-x)(1+2x+x^2-x^3)) */

#include <stdlib.h>

static seq_t *memo = NULL;          /* cache of previously computed terms   */
static idx_t  memo_size = 0;        /* number of elements stored (max n+1)  */

/* extend the cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < memo_size)
        return;                       /* already computed */

    /* allocate new storage */
    seq_t *new_mem = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_mem)                     /* out of memory: abort */
        abort();

    /* copy existing data */
    for (idx_t i = 0; i < memo_size; ++i)
        new_mem[i] = memo[i];

    free(memo);
    memo = new_mem;
    memo_size = n + 1;

    /* initialise mandatory starting values if they are missing */
    if (n >= 0 && memo[0] == 0)       /* not yet initialised */
        memo[0] = 1;
    if (n >= 1)                       /* a(1) */
        memo[1] = (seq_t)(-1);
    if (n >= 2)                       /* a(2) */
        memo[2] = 2;
    if (n >= 3)                       /* a(3) */
        memo[3] = (seq_t)(-1);

    /* compute remaining terms using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = -memo[i - 1];                /* -(a(n-1)) */
        seq_t t2 =  memo[i - 2];                /*  a(n-2)  */
        seq_t t3 =  memo[i - 3] * (seq_t)2;    /*  2*a(n-3)*/
        seq_t t4 = -memo[i - 4];                /* -(a(n-4))*/
        memo[i] = t1 + t2 + t3 + t4;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return memo[n];
}
