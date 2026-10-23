/*
 * A105210  a(1) = 393; for n > 1, a(n) = a(n-1) + 1 + sum of distinct prime
 *          factors of a(n-1) that are < a(n-1). edit.
 * https://oeis.org/A105210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1 + Σ p, where the sum is over distinct prime
 *            factors p of a(n-1) with p < a(n-1); a(1) = 393
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* sum of distinct prime factors of v that are strictly less than v
   (i.e. v's non‑trivial prime divisors) */
static seq_t sum_distinct_prime_factors_less(seq_t v)
{
    seq_t sum = 0;
    seq_t rem = v;

    for (seq_t p = (seq_t)2; p <= rem / p; ++p)
    {
        if (rem % p == 0)
        {
            sum += p;
            while (rem % p == 0)
                rem /= p;
        }
    }

    /* rem may be 1 or a prime factor > sqrt(v) */
    if (rem > 1 && rem != v)
        sum += rem;

    return sum;
}

/* memoisation of previously computed terms */
static seq_t *memo = NULL;
static idx_t memo_cap  = 0;
static idx_t memo_last = 0;

/* ensure the memo array can hold values up to index n */
static void ensure_memo_capacity(idx_t n)
{
    if (memo_cap >= n + 1)
        return;

    idx_t new_cap = memo_cap ? memo_cap * 2 : 4;
    if (new_cap < n + 1)
        new_cap = n + 1;

    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem)
        return;                                 /* allocation failure – caller will handle */

    memo = new_mem;
    memo_cap = new_cap;

    /* initialise the first term on first allocation */
    if (memo_last == 0)
    {
        memo[1] = (seq_t)393;
        memo_last = 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    ensure_memo_capacity(n);

    for (idx_t i = memo_last + 1; i <= n; ++i)
    {
        seq_t prev = memo[i - 1];
        seq_t add  = 1 + sum_distinct_prime_factors_less(prev);
        memo[i] = prev + add;
    }

    memo_last = n;
    return memo[n];
}
