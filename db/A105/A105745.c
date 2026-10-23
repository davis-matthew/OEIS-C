/*
 * A105745  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1, a(2)=12.
 * https://oeis.org/A105745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(1) = 1, a(2) = 12
   For n > 2, a(n) is the minimal positive integer such that
   a(n-1)^2 + 4*a(n-2)*a(n) is a perfect square. */

#include <stdlib.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n < 1 */

    /* static memoization storage */
    static seq_t *memo = NULL;     /* array of terms, indexed from 1 */
    static idx_t memo_len = 0;     /* largest index that has been computed */
    static idx_t memo_cap = 0;     /* current array capacity */

    /* initialise storage on first call */
    if (!memo) {
        memo_cap = 64;             /* arbitrary initial capacity */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0;       /* allocation failed */
    }

    /* grow storage if necessary */
    if (n >= memo_cap) {
        idx_t new_cap = memo_cap > 0 ? memo_cap * 2 : 64;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failed */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = memo_len + 1; i <= n; ++i) {
        if (i == 1) {               /* base cases */
            memo[i] = 1;
            continue;
        }
        if (i == 2) {
            memo[i] = 12;
            continue;
        }

        /* recurrence: find minimal a(i) */
        seq_t prev1 = memo[i - 1];
        seq_t prev2 = memo[i - 2];
        seq_t val = 1;              /* start at 1 and increase until square */

        while (1) {
            seq_t s = prev1 * prev1 + ((seq_t)4 * prev2 * val);
            seq_t k = isqrt(s);
            if (k * k == s) break; /* perfect square found */
            val++;
        }
        memo[i] = val;
    }

    memo_len = n;
    return memo[n];
}
