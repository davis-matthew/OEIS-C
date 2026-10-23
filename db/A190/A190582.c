/*
 * A190582  Generalized McCarthy function: a(n) = n - s if n > c; otherwise,
 *          a(n) = a(a(n+t)) with d = t - s > 0, with parameters t=15, s=9,
 *          c=21.
 * https://oeis.org/A190582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 9 if n > 21 * a(n) = a(a(n + 15)) otherwise *
 *            (Generalized McCarthy function with parameters t = 15, s = 9, c
 *            = 21) * * The implementation evaluates the recursion with
 *            memoisation for * all n ≤ 21. For n > 21 the result is computed
 *            directly as n-9
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define T 15
#define S 9
#define C 21

/*  Cache for results with n ≤ C; indices 0..C. */
static seq_t cache[C + 1];
static bool cache_valid[C + 1];

/*  Recursive evaluation of a(n). */
static seq_t rec(idx_t n)
{
    if (n > (idx_t)C)
        return (seq_t)n - S;

    if (cache_valid[n])
        return cache[n];

    seq_t inner = rec(n + T);
    seq_t result = rec((idx_t)inner);
    cache[n] = result;
    cache_valid[n] = true;
    return result;
}

/*  Public function as required. */
seq_t a(idx_t n)
{
    return rec(n);
}
