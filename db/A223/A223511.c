/*
 * A223511  Triangle T(n,k) represents the coefficients of (x^9*d/dx)^n, where
 *          n=1,2,3,...;generalization of Stirling numbers of second kind
 *          A008277, Lah-numbers A008297.
 * https://oeis.org/A223511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Let t_r = r(r+1)/2 (triangular numbers). For n with t_{r-1} < n
 *            < t_r : k = n - t_{r-1} (1 ≤ k ≤ r-1) a(n) = L(r,k) For n = t_r
 *            : a(n) = 1 The numbers L(r,k) (the "generalised Lah numbers" for
 *            m=9) satisfy L(1,1) = 1 L(r,k) = (k + 8*(r-1))*L(r-1,k) +
 *            L(r-1,k-1) (1 ≤ k ≤ r) These values produce ex...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  static cache of the L(r,k) values  */
static seq_t **L = NULL;          /* array of pointers to rows  */
static idx_t   precomp = 0;       /* largest r currently computed */

/*  Ensure that rows 1..r of L are available  */
static void ensure_precomp(idx_t r)
{
    if (r <= precomp) return;

    /*  allocate space for row pointers  */
    size_t new_size = r + 1;               /* indices 0..r  */
    if (L == NULL) {
        L = (seq_t**)malloc(new_size * sizeof(seq_t*));
    } else {
        L = (seq_t**)realloc(L, new_size * sizeof(seq_t*));
    }
    if (L == NULL) return;                 /* allocation failure */

    /*  allocate and compute missing rows  */
    for (idx_t i = precomp + 1; i <= r; ++i) {
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (row == NULL) return;           /* allocation failure */

        row[0] = 0;                        /* unused at index 0 */

        if (i == 1) {
            row[1] = 1;                    /* base case L(1,1) = 1 */
        } else {
            for (idx_t k = 1; k <= i; ++k) {
                seq_t prev_same = (k <= i - 1) ? L[i-1][k] : 0;
                seq_t prev_prev = (k-1 >= 1)   ? L[i-1][k-1] : 0;
                seq_t factor = (seq_t)k + (seq_t)(8 * (i-1));   /* (k + 8*(i-1)) */
                row[k] = prev_same * factor + prev_prev;
            }
        }
        L[i] = row;
    }
    precomp = r;                          /* update largest computed r */
}

/*  main sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; undefined below 1 */

    /*  find the smallest r with t_r >= n  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    idx_t t_r = (r * (r + 1)) / 2;
    if (n == t_r) return (seq_t)1;        /* triangular indices give 1 */

    idx_t t_prev = (r - 1) * r / 2;       /* t_{r-1} */
    idx_t k = n - t_prev;                /* 1 ≤ k ≤ r-1 */

    ensure_precomp(r);                    /* make sure L(r,*) is ready */

    return L[r][k];
}
