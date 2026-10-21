/*
 * A238510  Number of (n+1)X(1+1) 0..3 arrays with no element equal to all
 *          horizontal neighbors or equal to all vertical neighbors, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A238510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A238510:
   a(1) = 4,  a(2) = 25,  a(3) = 295,  a(4) = 3113,  a(5) = 33837,
   a(n) = 10*a(n-1) + 13*a(n-2) - 44*a(n-3) - 7*a(n-4) + 14*a(n-5)   for n > 5. */
#include <stdlib.h>

static seq_t *memo = NULL;   /* memoised sequence values */
static idx_t memo_size = 0;  /* largest index currently stored (0 = none) */

/* ensure memory for indices 0..up_to (inclusive) */
static void ensure_memo(idx_t up_to)
{
    if (up_to <= memo_size) return;
    idx_t new_size = up_to + 1;              /* indices 0..up_to */
    seq_t *new_mem = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_mem) abort();                  /* out of memory */

    /* copy old contents */
    if (memo) {
        for (idx_t i = 0; i <= memo_size; ++i)
            new_mem[i] = memo[i];
    }
    free(memo);
    memo = new_mem;
    /* memo_size is updated by the caller after computing new terms */
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined below offset */

    /* initialise the first five terms on first use */
    if (memo_size < 5) {
        ensure_memo(5);                      /* indices 0..5 */
        memo[1] = 4;
        memo[2] = 25;
        memo[3] = 295;
        memo[4] = 3113;
        memo[5] = 33837;
        memo_size = 5;
    }

    if (n > memo_size) {
        idx_t old_size = memo_size;          /* highest known index before growth */
        ensure_memo(n);                      /* allocate up to n */

        for (idx_t i = old_size + 1; i <= n; ++i) {
            seq_t a1 = memo[i - 1];
            seq_t a2 = memo[i - 2];
            seq_t a3 = memo[i - 3];
            seq_t a4 = memo[i - 4];
            seq_t a5 = memo[i - 5];

            /* use unsigned arithmetic; intermediate signed results are safe for the limits */
            seq_t term1 = (seq_t)10 * a1;    /* 10*a(i-1) */
            seq_t term2 = (seq_t)13 * a2;    /* 13*a(i-2) */
            seq_t term3 = (seq_t)44 * a3;    /* 44*a(i-3) */
            seq_t term4 = (seq_t)7  * a4;    /* 7*a(i-4) */
            seq_t term5 = (seq_t)14 * a5;    /* 14*a(i-5) */

            seq_t pos = term1 + term2 + term5;
            seq_t neg = term3 + term4;
            memo[i] = pos - neg;            /* final value fits in seq_t for all n >= 1 */
        }
        memo_size = n;
    }

    return memo[n];
}
