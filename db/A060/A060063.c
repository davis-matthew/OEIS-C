/*
 * A060063  Triangle of coefficients of certain polynomials used for G.f.s of
 *          columns of triangle A060058.
 * https://oeis.org/A060063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for A060063 (OEIS A060063)
 *
 *  The triangle of coefficients a(n,m) of the polynomials
 *  p(n,x) satisfies, for n≥1,
 *
 *      p(n,x) = x*(1‑x)^2 * p''(n‑1,x)
 *             + (1+6*(n‑1)*x+(5‑6*n)*x^2) * p'(n‑1,x)
 *             + (3*n‑2)*(1+(3*n‑2)*x) * p(n‑1,x)
 *
 *  with p(0,x)=1.  The flattened sequence is obtained by listing the
 *  coefficients of each row from left to right, starting with a(0)=1.
 *
 *  The function below constructs the rows iteratively and caches all
 *  generated coefficients; it can answer any a(n) in O(1) after an
 *  O(n) pre‑computation.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *flatten  = NULL;   /* flattened coefficients */
    static size_t fcap      = 0;     /* capacity of the flattened array   */
    static size_t fsize     = 0;     /* current number of stored coeffs   */

    static seq_t *prev_row = NULL;   /* coefficients of the previous row */
    static size_t prowlen   = 0;     /* number of coefficients in prev_row   */
    static idx_t  currow    = 0;     /* index of the last computed row      */

    /* --- initialise the first row (p(0,x)=1) --- */
    if (!flatten) {
        fcap   = 64;
        flatten = (seq_t *)malloc(fcap * sizeof(seq_t));
        if (!flatten) exit(EXIT_FAILURE);

        prev_row = (seq_t *)malloc(sizeof(seq_t));
        if (!prev_row) exit(EXIT_FAILURE);
        prev_row[0] = (seq_t)1;
        prowlen     = 1;

        flatten[0] = (seq_t)1;
        fsize      = 1;
        currow      = 0;
    }

    /* --- build rows until the array contains index n --- */
    while (fsize <= (size_t)n) {
        idx_t   next   = currow + 1;
        size_t next_len = (size_t)next + 1;   /* length of new row    */

        seq_t *curr_row = (seq_t *)calloc(next_len, sizeof(seq_t));
        if (!curr_row) exit(EXIT_FAILURE);

        /* compute coefficients of row next from row currow       */
        for (size_t k = 0; k < next_len; ++k) {
            seq_t pos = 0, neg = 0;

            /*  (k+1)*k   * a_{k+1} */
            if (k + 1 < prowlen) {
                seq_t t = prev_row[k + 1];
                t *= (seq_t)(k + 1);
                t *= (seq_t)k;
                pos += t;
            }
            /* - 2*k*(k-1) * a_k      */
            if (k < prowlen && k >= 2) {
                seq_t t = prev_row[k];
                t *= (seq_t)k;
                t *= (seq_t)(k - 1);
                t *= (seq_t)2;
                neg += t;
            }
            /* + (k-1)*(k-2) * a_{k-1} */
            if (k >= 3 && (k - 1) < prowlen) {
                seq_t t = prev_row[k - 1];
                t *= (seq_t)(k - 1);
                t *= (seq_t)(k - 2);
                pos += t;
            }
            /* + (k+1) * a_{k+1}       */
            if (k + 1 < prowlen) {
                seq_t t = prev_row[k + 1];
                t *= (seq_t)(k + 1);
                pos += t;
            }
            /* + 6*(next-1)*k * a_k    */
            if (k < prowlen) {
                seq_t t = prev_row[k];
                t *= (seq_t)k;
                t *= (seq_t)(6 * (next - 1));
                pos += t;
            }
            /* - (6*next-5)*(k-1) * a_{k-1} */
            if (k >= 1 && (k - 1) < prowlen) {
                seq_t t = prev_row[k - 1];
                t *= (seq_t)(k - 1);
                t *= (seq_t)(6 * next - 5);
                neg += t;
            }
            /* + (3*next-2) * a_k      */
            if (k < prowlen) {
                seq_t t = prev_row[k];
                t *= (seq_t)(3 * next - 2);
                pos += t;
            }
            /* + (3*next-2)^2 * a_{k-1} */
            if (k >= 1 && (k - 1) < prowlen) {
                seq_t t = prev_row[k - 1];
                seq_t m = (seq_t)(3 * next - 2);
                t *= m;
                t *= m;
                pos += t;
            }

            curr_row[k] = pos - neg;
        }

        /* ensure capacity of the flattened array */
        size_t needed = fsize + next_len;
        if (fcap < needed) {
            size_t newcap = fcap ? fcap << 1 : 64;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(flatten, newcap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            flatten = tmp;
            fcap   = newcap;
        }

        /* append the new row to the flattened array */
        for (size_t k = 0; k < next_len; ++k)
            flatten[fsize + k] = curr_row[k];
        fsize += next_len;

        /* advance to the next row */
        free(prev_row);
        prev_row = curr_row;
        prowlen = next_len;
        currow   = next;
    }

    return flatten[(size_t)n];
}
