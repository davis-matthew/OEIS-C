/*
 * A104554  Expansion of x*(1-x)/(1-x+2*x^3-x^4).
 * https://oeis.org/A104554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 2*a(n-3) + a(n-4), with a(0)=0, a(1)=1, a(2)=0,
 *            a(3)=0
 * Range:     i8 n <= 24, i16 n <= 46, i32 n <= 92, i64 n <= 185,
 *            i128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;          /* array of computed terms          */
static size_t memo_size = 0;        /* allocated size of memo           */
static size_t computed = 0;         /* number of entries already set    */

/* make sure memo has space for index n (inclusive) */
static void ensure_size(size_t n)
{
    if (n < memo_size) return;
    size_t new_size = memo_size ? memo_size : 4;
    while (new_size <= n) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure – terminate */
    memo = tmp;
    memo_size = new_size;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_size(idx);

    /* compute any missing terms up to n */
    if (idx >= computed) {
        for (size_t i = computed; i <= idx; ++i) {
            if (i < 4) {
                memo[i] = (i == 1) ? (seq_t)1 : (seq_t)0;
            } else {
                memo[i] = memo[i-1] - (seq_t)2 * memo[i-3] + memo[i-4];
            }
        }
        computed = idx + 1;
    }
    return memo[idx];
}
