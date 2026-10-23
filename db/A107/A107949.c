/*
 * A107949  Smallest k_n such that there exist positive integers 0 < k_1 < ...
 *          < k_n such that there exists only one n-tuple of nonnegative
 *          integers (b_1, ..., b_n) - namely (1, ..., 1) - such that the sum
 *          of the b_i's equals n and the sum of the b_i*k_i's equals the sum
 *          of the k_i's.
 * https://oeis.org/A107949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(1)=1; for n≥2, a(n)=2*a(n-1) if n is odd, a(n)=2*a(n-1)-1 if n
 *            is even and n>2, a(2)=2. (Minimal maximum element of a set
 *            {k_1<…<k_n} with unique representation property.)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation array (1‑based index) */
    static seq_t *memo = NULL;
    static size_t memo_len = 0;   /* last valid index stored */
    static size_t memo_cap = 0;   /* allocated capacity */

    /* initialise */
    if (memo_cap == 0) {
        memo_cap = 16;
        memo = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;       /* fallback on OOM */
        memo[0] = (seq_t)0;               /* dummy for 0‑index */
        memo[1] = (seq_t)1;               /* a(1) = 1 */
        memo_len = 1;
    }

    /* grow until we have a(n) */
    while (memo_len < n) {
        /* ensure capacity */
        if (memo_len + 1 >= memo_cap) {
            memo_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(memo, memo_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* fallback on OOM */
            memo = tmp;
        }

        idx_t i = memo_len + 1;          /* index to compute */
        seq_t prev = memo[i-1];
        seq_t val;

        if (i == 2) {
            val = prev + prev;           /* a(2) = 2*a(1) */
        } else if (i % 2 == 1) {
            val = prev + prev;           /* odd n: a(n) = 2*a(n-1) */
        } else {
            val = prev + prev - (seq_t)1;/* even n>2: a(n) = 2*a(n-1)-1 */
        }

        memo[++memo_len] = val;
    }

    return memo[n];
}
