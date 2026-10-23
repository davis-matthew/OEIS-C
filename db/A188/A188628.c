/*
 * A188628  a(n) = smallest k such that k*2^n - 7 is a square.
 * https://oeis.org/A188628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that k*2^n - 7 is a square (a(0)=7,
 *            a(1)=4, a(2)=2, a(3)=1, ...). Recurrence: if a(n-1) is even ->
 *            a(n)=a(n-1)/2 , b(n)=b(n-1) if a(n-1) is odd -> b(n) = 2^(n-2) -
 *            b(n-1) a(n) = 2^(n-3) + (a(n-1)>>1) - (b(n-1)>>1) where b(n) is
 *            an integer satisfying b(n)^2 ≡ -7 (mod 2^...
 * Range:     u8 n <= 15, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* for size_t */
#include "../utils/pow2.h"          /* pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Memoization storage */
    static seq_t *a_tbl   = NULL;
    static seq_t *b_tbl   = NULL;
    static size_t cap     = 0;
    static idx_t  cnt     = 0;    /* highest index stored + 1 */

    /* Ensure capacity */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        a_tbl = (seq_t *)realloc(a_tbl, new_cap * sizeof(seq_t));
        b_tbl = (seq_t *)realloc(b_tbl, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Initialize base term a(0)=7 */
    if (cnt == 0) {
        a_tbl[0] = (seq_t)7;
        b_tbl[0] = (seq_t)0;          /* unused for n=0 */
        cnt = 1;
    }

    if (n == 0) return a_tbl[0];

    /* Compute terms up to n */
    for (idx_t i = cnt; i <= n; ++i) {
        if (i == 1) {
            a_tbl[1] = (seq_t)4;
            b_tbl[1] = (seq_t)1;
            continue;
        }

        seq_t a_prev = a_tbl[i-1];
        seq_t b_prev = b_tbl[i-1];
        seq_t a_next, b_next;

        if ((a_prev & (seq_t)1) == 0) {          /* a_prev even */
            a_next = a_prev >> 1;
            b_next = b_prev;
        } else {                                 /* a_prev odd   */
            /* Compute new b: 2^(i-2) - b_prev */
            seq_t pow_n2 = pow2(i-2);            /* 2^(i-2) */
            b_next = pow_n2 - b_prev;

            /* Compute new a: 2^(i-4) + (a_prev>>1) - (b_prev>>1) */
            seq_t pow_n4 = pow2(i-4);            /* 2^(i-4)  */
            a_next = pow_n4 + (a_prev >> 1);
            a_next -= (b_prev >> 1);
        }

        a_tbl[i] = a_next;
        b_tbl[i] = b_next;
    }

    cnt = n + 1;
    return a_tbl[n];
}
