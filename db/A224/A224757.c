/*
 * A224757  a(2)=6; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 6. For n > 1, let P = a(n-1). a(n) is the smallest m such
 *            that P + m = (P followed by the leading digit of m).
 *            Equivalently, find the smallest digit d ∈ {1,…,9} with
 *            leading_digit(9·P + d) = d; then a(n) = 9·P + d
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 27,
 *            big any n
 * Helpers:   digit_len, pow10, seq_from_str, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the terms so that calls may be
   in any order.  It uses standard C library allocation. */

#include <stdlib.h>
#include <stdbool.h>

/* fetch helper from utils */
#include "../utils/seq_pow.h"
#include "../utils/seq_from_str.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Forward declaration of helper that computes the next term */
static seq_t _next_value(seq_t prev)
{
    seq_t X = prev * (seq_t)9;           /* X = 9 * prev */
    for (int d = 1; d <= 9; ++d)
    {
        seq_t m   = X + (seq_t)d;
        int   dig = digit_len(m);          /* number of decimal digits of m */
        seq_t pow = pow10((idx_t)(dig - 1)); /* 10^(dig-1) */
        seq_t ld  = m / pow;               /* leading digit of m */
        if (ld == (seq_t)d)
            return m;
    }
    /* According to the definition this should never happen */
    return (seq_t)0;
}

/* Memoised sequence generator */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* 1-based index; cache[0] unused */
    static idx_t  cache_len = 0;       /* current allocated length, excludes index 0 */

    if (n == 0)
        return (seq_t)0;               /* sequence is undefined for n < 1 */

    /* grow cache if needed */
    if (n > cache_len)
    {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp)
            abort();                   /* memory allocation failed */
        cache = tmp;

        /* fill new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            if (i == 1)
                cache[i] = (seq_t)6;
            else
                cache[i] = _next_value(cache[i - 1]);
        }
        cache_len = n;
    }
    return cache[n];
}
