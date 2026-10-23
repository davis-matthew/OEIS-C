/*
 * A106523  Diagonal sums of number triangle A106522.
 * https://oeis.org/A106523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=10, a(5)=14
   a(n) = 3*a(n-2) + a(n-3) - a(n-5)   for n >= 5
   (Both the initial values and the recurrence are listed in the comment.) */

#include <stdlib.h>

static seq_t *memo = NULL;      /* cache of computed terms */
static idx_t  memo_size = 0;    /* number of cached terms */

seq_t a(idx_t n)
{
    /* initialise base values on first call */
    if (memo_size == 0) {
        const idx_t base_count = 6; /* terms 0..5 inclusive */
        memo = (seq_t *)malloc(base_count * sizeof(seq_t));
        if (!memo) return (seq_t)0; /* allocation failed – unlikely */

        memo[0] = (seq_t)1;   /* a(0) */
        memo[1] = (seq_t)1;   /* a(1) */
        memo[2] = (seq_t)3;   /* a(2) */
        memo[3] = (seq_t)4;   /* a(3) */
        memo[4] = (seq_t)10;  /* a(4) */
        memo[5] = (seq_t)14;  /* a(5) */
        memo_size = base_count;
    }

    /* return pre‑computed term */
    if (n < memo_size) return memo[n];

    /* extend the cache up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* allocation failed – unlikely */
    memo = tmp;

    for (idx_t i = memo_size; i <= n; ++i) {
        /* a(i) = 3*a(i-2) + a(i-3) – a(i-5) */
        seq_t val = 3 * memo[i - 2];
        val += memo[i - 3];
        val -= memo[i - 5];
        memo[i] = val;
    }
    memo_size = new_size;

    return memo[n];
}
