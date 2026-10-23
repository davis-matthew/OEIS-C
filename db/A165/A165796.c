/*
 * A165796  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165796 – Number of reduced words of length n in the Coxeter group on 11 generators
 *
 * Recurrence (OEIS):
 *   a(0) = 1
 *   a(1) = 11
 *   a(n) = 10 * a(n‑1)          for 2 ≤ n ≤ 9
 *   a(n) = 9 * (a(n‑1)+…+a(n‑9)) − 45 * a(n‑10) + δ_{n,10}
 *   where δ_{n,10} = 1 if n = 10, else 0.
 *
 * The first nine terms form a geometric progression of ratio 10.
 *
 * The implementation caches computed terms so that any order of calls
 * is handled efficiently.  It works for all declared seq_t/idx_t
 * types (fixed‑width integers, arbitrary‑precision integers, etc.).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *values  = NULL;
    static idx_t  stored  = 0;   /* number of terms already computed */
    static size_t capacity = 0;  /* allocated array size */

    /* Ensure the allocator has room for (n+1) entries */
    if ((size_t)n + 1 > capacity) {
        size_t newcap = capacity ? capacity * 2 : 16;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(values, newcap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – return 0 */
        values   = tmp;
        capacity = newcap;
    }

    /* Compute any missing terms up to n */
    if (stored <= n) {
        for (idx_t i = stored; i <= n; ++i) {
            if (i == 0) {
                values[0] = (seq_t)1;
            } else if (i == 1) {
                values[1] = (seq_t)11;
            } else if (i <= 9) {                      /* 10‑fold geometric progression */
                values[i] = values[i - 1] * (seq_t)10;
            } else {                                 /* linear recurrence for i ≥ 10 */
                seq_t sum = 0;
                for (int k = 1; k <= 9; ++k)
                    sum += values[i - k];
                seq_t val = (seq_t)9 * sum - (seq_t)45 * values[i - 10];
                if (i == 10) val += (seq_t)1;        /* constant term only for n = 10   */
                values[i] = val;
            }
        }
        if (n + 1 > stored) stored = n + 1;          /* update cached boundary   */
    }

    return values[n];
}
