/*
 * A108057  G.f.
 *          (x-1)*(x^2+1)*(x^7-x^6+x^4+x^3-2*x^2-x-1)/((x^2-x+1)*(x^6-x^3+1)*(x+1)^2).
 * https://oeis.org/A108057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(a(n-1)+a(n-9)+a(n-10)) for n >= 11; a(10) = 1 - a(9) -
 *            a(1) - a(0); Initial terms: a(0)=1, a(1)=-1, a(2)=3, a(3)=-6,
 *            a(4)=7, a(5)=-9, a(6)=10, a(7)=-11, a(8)=13, a(9)=-16
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_memo = NULL;
static idx_t a_memo_size = 0;
static idx_t a_memo_cap  = 0;

static void a_ensure_cap(idx_t needed)
{
    if (a_memo_cap < needed) {
        idx_t new_cap = a_memo_cap ? a_memo_cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(a_memo, new_cap * sizeof(seq_t));
        a_memo = tmp;
        a_memo_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialize memoization table with the first 10 terms */
    if (a_memo_size == 0) {
        a_ensure_cap(10);
        a_memo[0] = (seq_t)  1;
        a_memo[1] = (seq_t) -1;
        a_memo[2] = (seq_t)  3;
        a_memo[3] = (seq_t) -6;
        a_memo[4] = (seq_t)  7;
        a_memo[5] = (seq_t) -9;
        a_memo[6] = (seq_t) 10;
        a_memo[7] = (seq_t)-11;
        a_memo[8] = (seq_t) 13;
        a_memo[9] = (seq_t)-16;
        a_memo_size = 10;
    }

    if (n < a_memo_size) return a_memo[(size_t)n];

    a_ensure_cap(n + 1);

    for (idx_t k = a_memo_size; k <= n; ++k) {
        if (k == 10) {
            a_memo[k] = (seq_t)1
                        - a_memo[k-1]   /* a(9)  */
                        - a_memo[1]     /* a(1)  */
                        - a_memo[0];    /* a(0)  */
        } else if (k >= 11) {
            a_memo[k] = -(a_memo[k-1] + a_memo[k-9] + a_memo[k-10]);
        }
        /* k < 10 is impossible here */
    }

    a_memo_size = n + 1;
    return a_memo[(size_t)n];
}
