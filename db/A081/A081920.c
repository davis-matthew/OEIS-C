/*
 * A081920  Expansion of exp(2x)/sqrt(1-x^2).
 * https://oeis.org/A081920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + (n-1)^2 * a(n-2) - 2*(n-1)*(n-2) * a(n-3), *
 *            with a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;      /* array holding computed terms */
static idx_t memo_capacity = 0; /* allocated size of memo */
static idx_t memo_len = 0;      /* number of terms actually computed (next free index) */

static void ensure_capacity(idx_t n)
{
    if (memo_capacity > n) return;
    idx_t new_cap = memo_capacity ? memo_capacity : 8;
    while (new_cap <= n) new_cap <<= 1U;
    seq_t *new_mem = (seq_t *)realloc(memo, (size_t)new_cap * sizeof(seq_t));
    if (!new_mem) abort();
    for (idx_t i = memo_capacity; i < new_cap; ++i) new_mem[i] = 0;
    memo = new_mem;
    memo_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < memo_len) return memo[n];
    ensure_capacity(n);
    for (idx_t i = memo_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)2;
        } else if (i == 2) {
            val = (seq_t)5;
        } else {
            seq_t term1 = memo[i-1] * (seq_t)2;
            seq_t ip1 = (seq_t)(i - 1);
            seq_t ip2 = (seq_t)(i - 2);
            seq_t term2 = memo[i-2] * (ip1 * ip1);
            seq_t term3 = memo[i-3] * ((ip1 * ip2) * (seq_t)2);
            val = term1 + term2 - term3;
        }
        memo[i] = val;
    }
    memo_len = n + 1;
    return memo[n];
}
