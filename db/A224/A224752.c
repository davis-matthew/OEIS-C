/*
 * A224752  a(1)=1; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; Recurrence: for n > 1 let d be the smallest 1 ≤ d ≤ 9
 *            with leading_digit(9*a(n-1) + d) == d; then a(n) = 9*a(n-1) + d
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   digit_len, mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file relies on the following utilities, which supply
   generic operations for seq_t types:
   - pow10.h   : seq_t pow10(idx_t e);
   - digit_len.h : int digit_len(seq_t x);
   - mul_small.h : seq_t mul_small(seq_t x, unsigned int k); */

#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/mul_small.h"

/* Return the most significant decimal digit of x (x > 0). */
static int leading_digit(seq_t x)
{
    int len = digit_len(x);                 /* x > 0 by construction   */
    seq_t div = pow10((idx_t)(len - 1));    /* 10^(len-1)              */
    return (int)(x / div);                  /* top digit 1..9           */
}

/* Memoisation cache for the sequence: cache[i] holds a(i+1). */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;   /* number of terms actually stored   */
static idx_t cache_cap   = 0;   /* allocated capacity (in terms)     */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unsigned index won't reach here */

    /* Initialise cache with first term a(1)=1 if empty. */
    if (cache_len == 0) {
        cache_cap = (n > 16 ? n : 16);
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;      /* allocation failure      */
        cache[0] = (seq_t)1;              /* a(1) = 1                */
        cache_len = 1;
    }

    /* Grow cache if necessary. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure      */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to a(n). */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t nine_prev = mul_small(prev, 9);   /* 9 * a(i) */

        int chosen_d = 0;
        for (int d = 1; d <= 9; ++d) {
            seq_t m_candidate = nine_prev + (seq_t)d;
            if (leading_digit(m_candidate) == d) {
                chosen_d = d;
                break;
            }
        }
        /* m = 9*prev + d */
        cache[i] = nine_prev + (seq_t)chosen_d;
    }
    cache_len = n;

    return cache[n - 1];
}
