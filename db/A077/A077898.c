/*
 * A077898  Expansion of (1 - x)^(-1)/(1 + x - 2*x^2).
 * https://oeis.org/A077898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - 2*a(n-3) (for n>2) Initial values: a(0)=1,
 *            a(1)=0, a(2)=3
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;          /* array of computed terms */
static size_t capacity = 0;         /* allocated size of memo */
static idx_t  max_computed = 0;     /* highest n for which memo[n] is valid */

/* grow memo to hold at least new_cap elements */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= capacity) return;
    size_t newsize = capacity ? capacity : 64;
    while (newsize < new_cap) newsize <<= 1;
    seq_t *newp = (seq_t*)realloc(memo, newsize * sizeof(seq_t));
    if (!newp) exit(1);               /* allocation failure */
    memo = newp;
    capacity = newsize;
}

/* initialise base terms if this is the first call */
static void init_base(void)
{
    if (memo) return;                 /* already initialised */
    ensure_capacity(64);
    memo[0] = 1;
    memo[1] = 0;
    memo[2] = 3;
    max_computed = 2;
}

seq_t a(idx_t n)
{
    init_base();

    if (n <= max_computed) return memo[(size_t)n];

    /* grow memo to accommodate n */
    ensure_capacity((size_t)n + 1);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 3*a(i-2) - 2*a(i-3)   for i>2 */
        seq_t t1 = memo[(size_t)(i - 2)];
        t1 *= 3;
        seq_t t2 = memo[(size_t)(i - 3)];
        t2 *= 2;
        memo[(size_t)i] = t1 - t2;
    }
    max_computed = n;
    return memo[(size_t)n];
}
