/*
 * A169160  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(n)=3*2^(n-1) for 2≤n≤26,
 *            a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-…-a(n-26) for n≥27
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A169160.                */
seq_t a(idx_t n)
{
    /* static memoisation table                                        */
    static seq_t *memo = NULL;     /* array of computed terms          */
    static size_t mem_cap = 0;     /* capacity of the array           */
    static idx_t  mem_last = (idx_t)-1; /* last computed index */

    /* initialise the table on first use                                 */
    if (mem_last == (idx_t)-1) {
        mem_cap = 1;
        memo = (seq_t *)malloc(mem_cap * sizeof(seq_t));
        memo[0] = (seq_t)1;        /* a(0)=1 */
        mem_last = 0;
    }

    /* extend the table if necessary                                      */
    if (n + 1 > mem_cap) {
        size_t new_cap = mem_cap ? mem_cap * 2 : 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo = tmp;
        mem_cap = new_cap;
    }

    /* compute missing terms up to n                                      */
    for (idx_t k = mem_last + 1; k <= n; ++k) {
        if (k == 0) {
            memo[0] = (seq_t)1;
        } else if (k == 1) {
            memo[1] = (seq_t)3;
        } else if (k <= 26) {
            /* a(k) = 2*a(k-1) for 2 ≤ k ≤ 26 (doubles the previous term) */
            memo[k] = memo[k-1] << 1;
        } else {
            /* recurrence for k ≥ 27:
               a(k) = 2*a(k-1) - a(k-2) + 2*a(k-3) - … - a(k-26)          */
            seq_t pos = 0;   /* sum of odd‑index contributions (multiplied by 2)   */
            seq_t neg = 0;   /* sum of even‑index contributions (direct)           */
            for (idx_t i = 1; i <= 26; ++i) {
                if (i & 1) {
                    pos += memo[k-i] << 1;
                } else {
                    neg += memo[k-i];
                }
            }
            memo[k] = pos - neg;   /* pos ≥ neg for all k ≥ 27 (sequence is positive) */
        }
    }
    mem_last = n;
    return memo[n];
}
