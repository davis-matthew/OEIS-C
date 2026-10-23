/*
 * A187591  Number of 8-step E, S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>9 Initial values:
 *            a(1)=0, a(2)=0, a(3)=34, a(4)=2309, a(5)=15056, a(6)=45718,
 *            a(7)=95776, a(8)=164135, a(9)=250132
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 */

    /* Initial values for n ≤ 9 */
    static const seq_t base_vals[10] = {
        0,          /* base_vals[0] unused */
        0,          /* a(1) */
        0,          /* a(2) */
        34,         /* a(3) */
        2309,       /* a(4) */
        15056,      /* a(5) */
        45718,      /* a(6) */
        95776,      /* a(7) */
        164135,     /* a(8) */
        250132      /* a(9) */
    };

    /* Static cache for computed values */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;    /* allocated size (index 0 unused) */
    static idx_t max_computed = 0; /* largest n computed so far */

    /* Allocate cache on first call */
    if (memo == NULL) {
#ifdef __cplusplus
        memo = new seq_t[10]();           /* value‑initialize to zero */
#else
        memo = (seq_t*)calloc(10, sizeof(seq_t));
#endif
        memo_size = 10;
        for (idx_t i = 1; i <= 9; ++i) memo[i] = base_vals[i];
        max_computed = 9;
    }

    /* Grow the cache if needed */
    if (n + 1 > memo_size) {
#ifdef __cplusplus
        idx_t new_size = n + 1;
        seq_t *tmp = new seq_t[new_size]();  /* zero‑init */
        for (idx_t i = 0; i < memo_size; ++i) tmp[i] = memo[i];
        delete[] memo;
        memo = tmp;
        memo_size = new_size;
#else
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        memset(&tmp[memo_size], 0, (new_size - memo_size) * sizeof(seq_t));
        memo = tmp;
        memo_size = new_size;
#endif
    }

    /* Compute missing values using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 3 * memo[i - 1];
            val -= 3 * memo[i - 2];
            val += memo[i - 3];
            memo[i] = val;
        }
        max_computed = n;
    }

    return memo[n];
}
