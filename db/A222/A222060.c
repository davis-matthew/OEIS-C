/*
 * A222060  Triangle read by rows: coefficients of harmonic-geometric
 *          polynomials.
 * https://oeis.org/A222060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = Stirling2(n,k) * |Stirling1(k+1,2)|, where k = n -
 *            n(n+1)/2, i.e. row r and column k of the triangle read by rows
 *            (OEIS A222060)
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 84, u64 n <= 201,
 *            u128 n <= 545, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* cache for Stirling numbers of the second kind: S[i][j] = S(i,j) */
#define MAX_R 200
static seq_t S[MAX_R][MAX_R];
static bool S_inited = false;

/* cache for |Stirling1(m,2)| for m=1..MAX_R */
static seq_t abs_s[MAX_R];
static bool abs_s_inited = false;

/* initialize Stirling2 table up to MAX_R */
static void init_stirling2(void)
{
    if (S_inited) return;
    for (idx_t i = 0; i < MAX_R; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            if (j == 0) S[i][j] = (i == 0) ? (seq_t)1 : (seq_t)0;
            else if (j == i) S[i][j] = (seq_t)1;
            else
                S[i][j] = (seq_t)j * S[i - 1][j] + S[i - 1][j - 1];
        }
    }
    S_inited = true;
}

/* initialize abs_s array using recurrence:
   abs_s(1)=0, abs_s(2)=1,
   abs_s(m) = (m-1)*abs_s(m-1) + (m-2)!      for m >= 3
*/
static void init_abs_s(void)
{
    if (abs_s_inited) return;
    abs_s[1] = (seq_t)0;          /* |s(1,2)| = 0  */
    abs_s[2] = (seq_t)1;          /* |s(2,2)| = 1  */
    seq_t fact = (seq_t)1;        /* (m-2)! initially for m=3 */
    for (idx_t m = 3; m < MAX_R; ++m) {
        fact *= (seq_t)(m - 2);   /* (m-2)!  */
        abs_s[m] = (seq_t)(m - 1) * abs_s[m - 1] + fact;
    }
    abs_s_inited = true;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    /* determine the row r and column k for index n */
    idx_t r = 0;
    /* loop until (r+1)(r+2)/2 > n */
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;

    if (r >= MAX_R || k > r) return (seq_t)0;  /* safety overflow guard */

    init_stirling2();
    init_abs_s();

    seq_t stirl = S[r][k];
    seq_t abs_stirl = abs_s[k + 1];   /* |s(k+1,2)| */

    return stirl * abs_stirl;
}
